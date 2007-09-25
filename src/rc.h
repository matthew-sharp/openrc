/*!
 * @file rc.h 
 * @brief Describes how to interface with the RC library
 * @internal
 *
 * Copyright 2007 Gentoo Foundation
 * Released under the GPLv2
 */

#ifndef __RC_H__
#define __RC_H__

#ifdef __GNUC__
#  define GCC_VERSION (__GNUC__ * 1000 + __GNUC__MINOR)
#  if (GCC_VERSION >= 3005)
#    define SENTINEL __attribute__ ((__sentinel__))
#  endif
#endif
#ifndef SENTINEL
#  define SENTINEL
#endif

#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>

/*! @name Reserved runlevel names */
#define RC_LEVEL_SYSINIT    "sysinit"
#define RC_LEVEL_SINGLE     "single"
#define RC_LEVEL_SHUTDOWN   "shutdown"
#define RC_LEVEL_REBOOT     "reboot"

/*! @name rc_ls_dir options */
/*! Ensure that an init.d service exists for each file returned */
#define RC_LS_INITD	0x01

/*! @name RC
 * A service can be given as a full path or just its name.
 * If its just a name then we try to resolve the service to a full path.
 * This should allow the use if local init.d directories in the future. */

/*! @brief States a service can be in */
typedef enum
{
	rc_service_started,
	rc_service_stopped,
	rc_service_starting,
	rc_service_stopping,
	rc_service_inactive,
	rc_service_wasinactive,
	rc_service_coldplugged,
	rc_service_failed,
	rc_service_scheduled,
	rc_service_crashed
} rc_service_state_t;

/*! Resolves a service name to its full path.
 * @param service to check
 * @return pointer to full path of service */
char *rc_resolve_service (const char *service);
/*! Checks if a service exists or not.
 * @param service to check
 * @return true if service exists, otherwise false */
bool rc_service_exists (const char *service);

/*! Lists the extra options a service has
 * @param service to load the options from
 * @return NULL terminated string list of options */
char **rc_service_options (const char *service);

/*! Returns a description of what the service and/or option does.
 * @param service to check
 * @param option to check (if NULL, service description)
 * @return a newly allocated pointer to the description */
char *rc_service_description (const char *service, const char *option);

/*! Checks if a service is in a runlevel
 * @param service to check
 * @param runlevel it should be in
 * @return true if service is in the runlevel, otherwise false */
bool rc_service_in_runlevel (const char *service, const char *runlevel);

/*! Checks if a service in in a state
 * @param service to check
 * @param state service should be in
 * @return true if service is in the requested state, otherwise false */
bool rc_service_state (const char *service, rc_service_state_t state);

/*! Marks the service state
 * @param service to mark
 * @param state service should be in
 * @return true if service state change was successful, otherwise false */
bool rc_mark_service (const char *service, rc_service_state_t state);

/*! Stop a service
 * @param service to stop
 * @return pid of service stopping process */
pid_t rc_stop_service (const char *service);

/*! Start a service
 * @param service to start
 * @return pid of the service starting process */
pid_t rc_start_service (const char *service);

/*! Wait for a process to finish
 * @param pid to wait for
 * @return exit status of the process */
int rc_waitpid (pid_t pid); 

/*! Schedule a service to be started when another service starts
 * @param service that starts the scheduled service when started
 * @param service_to_start service that will be started */
void rc_schedule_start_service (const char *service,
								const char *service_to_start);
/*! Return a NULL terminated list of services that are scheduled to start
 * when the given service has started
 * @param service to check
 * @return NULL terminated list of services scheduled to start */
char **rc_services_scheduled_by (const char *service);

/*! Clear the list of services scheduled to be started by this service
 * @param service to clear */
void rc_schedule_clear (const char *service);

/*! Wait for a service to finish
 * @param service to wait for
 * @return true if service finished before timeout, otherwise false */
bool rc_wait_service (const char *service);

/*! Return a saved value for a service
 * @param service to check
 * @param option to load
 * @return saved value */
char *rc_get_service_option (const char *service, const char *option);
/*! Save a persistent value for a service
 * @param service to save for
 * @param option to save
 * @param value of the option
 * @return true if saved, otherwise false */
bool rc_set_service_option (const char *service, const char *option,
							const char *value);
/*! Save the arguments to find a running daemon
 * @param service to save arguments for
 * @param exec that we started
 * @param name of the process (optional)
 * @param pidfile of the process (optional)
 * @param started if true, add the arguments otherwise remove existing matching arguments */
void rc_set_service_daemon (const char *service, const char *exec,
							const char *name, const char *pidfile,
							bool started);
/*! Check if the service started the daemon
 * @param service to check
 * @param exec to check
 * @param indx of the daemon (optional - 1st daemon, 2nd daemon, etc)
 * @return true if started by this service, otherwise false */
bool rc_service_started_daemon (const char *service, const char *exec,
								int indx);

/*! Check if the service is allowed to be hot/cold plugged
 * @param service to check
 * @return true if allowed, otherwise false */
bool rc_allow_plug (char *service);

/*! Return the current runlevel.
 * @return the current runlevel */
char *rc_get_runlevel (void);
/*! Set the runlevel.
 * This just changes the stored runlevel and does not start or stop any services.
 * @param runlevel to store */
void rc_set_runlevel (const char *runlevel);

/*! Checks if the runlevel exists or not
 * @param runlevel to check
 * @return true if the runlevel exists, otherwise false */
bool rc_runlevel_exists (const char *runlevel);

/*! Return a NULL terminated list of runlevels
 * @return a NULL terminated list of runlevels */
char **rc_get_runlevels (void);

/*! Is the runlevel starting?
 * @return true if yes, otherwise false */
bool rc_runlevel_starting (void);
/*! Is the runlevel stopping?
 * @return true if yes, otherwise false */
bool rc_runlevel_stopping (void);

/*! Add the service to the runlevel
 * @param runlevel to add to
 * @param service to add
 * @return true if successful, otherwise false */
bool rc_service_add (const char *runlevel, const char *service);
/*! Remove the service from the runlevel
 * @param runlevel to remove from
 * @param service to remove
 * @return true if sucessful, otherwise false */
bool rc_service_delete (const char *runlevel, const char *service);
/*! List the services in a runlevel
 * @param runlevel to list
 * @return NULL terminated list of services */
char **rc_services_in_runlevel (const char *runlevel);
/*! List the services in a state
 * @param state to list
 * @return NULL terminated list of services */
char **rc_services_in_state (rc_service_state_t state);
/*! List the services shceduled to start when this one does
 * @param service to check
 * @return  NULL terminated list of services */
char **rc_services_scheduled (const char *service);

/*! Find processes based on criteria.
 * All of these are optional.
 * pid overrides anything else.
 * If both exec and cmd are given then we ignore exec.
 * @param exec to check for
 * @param cmd to check for
 * @param uid to check for
 * @param pid to check for
 * @return NULL terminated list of pids */
pid_t *rc_find_pids (const char *exec, const char *cmd,
					 uid_t uid, pid_t pid);
/*! Checks that all daemons started with start-stop-daemon by the service
 * are still running.
 * @param service to check
 * @return true if all daemons started are still running, otherwise false */
bool rc_service_daemons_crashed (const char *service);

/*! @name Dependency options
 * These options can change the services found by the rc_get_depinfo and
 * rc_get_depends functions. */
/*! Trace provided services */
#define RC_DEP_TRACE    0x01
/*! Only use services added to runlevels */
#define RC_DEP_STRICT   0x02
/*! Runlevel is starting */
#define RC_DEP_START    0x04
/*! Runlevel is stopping */
#define RC_DEP_STOP     0x08

/*! @name Dependencies
 * We analyse each init script and cache the resultant dependency tree.
 * This tree can be accessed using the below structures and functions. */
/*! Singly linked list of dependency types that list the services the
 * type is for */
typedef struct rc_deptype
{
	/*! ineed, iuse, iafter, etc */
	char *type;
	/*! NULL terminated list of services */
	char **services;
	/*! Next dependency type */
	struct rc_deptype *next;
} rc_deptype_t;

/*! Singly linked list of services and their dependencies */
typedef struct rc_depinfo
{
	/*! Name of service */
	char *service;
	/*! Dependencies */
	rc_deptype_t *depends;
	/*! Next service dependency type */
	struct rc_depinfo *next;
} rc_depinfo_t;

/*! Update the cached dependency tree if it's older than any init script,
 * its configuration file or an external configuration file the init script
 * has specified.
 * @param force an update
 * @return 0 if successful, otherwise -1 */
int rc_update_deptree (bool force);
/*! Load the cached dependency tree and return a pointer to it.
 * This pointer should be freed with rc_free_deptree when done.
 * @return pointer to the dependency tree */
rc_depinfo_t *rc_load_deptree (void);
/*! Get a services depedency information from a loaded tree
 * @param deptree to search
 * @param service to find
 * @return service dependency information */
rc_depinfo_t *rc_get_depinfo (rc_depinfo_t *deptree, const char *service);
/*! Get a depenency type from the service dependency information
 * @param depinfo service dependency to search
 * @param type to find
 * @return service dependency type information */
rc_deptype_t *rc_get_deptype (rc_depinfo_t *depinfo, const char *type);
char **rc_get_depends (rc_depinfo_t *deptree, char **types,
					   char **services, const char *runlevel, int options);
/*! List all the services that should be stoppned and then started, in order,
 * for the given runlevel, including sysinit and boot services where
 * approriate.
 * @param deptree to search
 * @param runlevel to change into
 * @param options to pass
 * @return NULL terminated list of services in order */
char **rc_order_services (rc_depinfo_t *deptree, const char *runlevel,
						  int options);
/*! Free a deptree and its information
 * @param deptree to free */
void rc_free_deptree (rc_depinfo_t *deptree);

/*! @name Plugins
 * For each plugin loaded we will call rc_plugin_hook with the below
 * enum and either the runlevel name or service name.
 *
 * Plugins are called when rc does something. This does not indicate an
 * end result and the plugin should use the above functions to query things
 * like service status.
 *
 * The service hooks have extra ones - now and done. This is because after
 * start_in we may start other services before we start the service in
 * question. now shows we really will start the service now and done shows
 * when we have done it as may start scheduled services at this point. */
/*! Points at which a plugin can hook into RC */
typedef enum
{
	rc_hook_runlevel_stop_in = 1,
	rc_hook_runlevel_stop_out = 4,
	rc_hook_runlevel_start_in = 5,
	rc_hook_runlevel_start_out = 8,
	/*! We send the abort if an init script requests we abort and drop
	 * into single user mode if system not fully booted */
	rc_hook_abort = 99,
	rc_hook_service_stop_in = 101,
	rc_hook_service_stop_now,
	rc_hook_service_stop_done,
	rc_hook_service_stop_out,
	rc_hook_service_start_in,
	rc_hook_service_start_now,
	rc_hook_service_start_done,
	rc_hook_service_start_out
} rc_hook_t;

/*! Plugin entry point
 * @param hook point
 * @param name of runlevel or service
 * @return 0 for success otherwise -1 */
int rc_plugin_hook (rc_hook_t hook, const char *name);

/*! Plugins should write FOO=BAR to this fd to set any environment
 * variables they wish. Variables should be separated by NULLs. */
extern FILE *rc_environ_fd;

/*! @name Memory Allocation
 * Ensure that if we cannot allocate the memory then we exit */
/*@{*/
/*! Allocate a block of memory
 * @param size of memory to allocate
 * @return pointer to memory */
void *rc_xmalloc (size_t size);
/*! Re-size a block of memory
 * @param ptr to the block of memory to re-size
 * @param size memory should be
 * @return pointer to memory block */
void *rc_xrealloc (void *ptr, size_t size);
/*! Duplicate a NULL terminated string
 * @param str to duplicate
 * @return pointer to the new string */
char *rc_xstrdup (const char *str);
/*@}*/

/*! @name Utility
 * Although not RC specific functions, they are used by the supporting
 * applications */
/*! Concatenate paths adding '/' if needed. The resultant pointer should be
 * freed when finished with.
 * @param path1 starting path
 * @param paths NULL terminated list of paths to add
 * @return pointer to the new path */
char *rc_strcatpaths (const char *path1, const char *paths, ...) SENTINEL;
/*! Check if an environment variable matches the given value
 * @param variable to check
 * @param value it should be
 * @return true if it matches */
bool rc_is_env (const char *variable, const char *value);
/*! Check if the file exists or not
 * @param pathname to check
 * @return true if it exists, otherwise false */
bool rc_exists (const char *pathname);
/*! Check if the file is a real file
 * @param pathname to check
 * @return true if it's a real file, otherwise false */
bool rc_is_file (const char *pathname);
/*! Check if the file is a symbolic link or not
 * @param pathname to check
 * @return true if it's a symbolic link, otherwise false */
bool rc_is_link (const char *pathname);
/*! Check if the file is a directory or not
 * @param pathname to check
 * @return true if it's a directory, otherwise false */
bool rc_is_dir (const char *pathname);
/*! Check if the file is marked executable or not
 * @param pathname to check
 * @return true if it's marked executable, otherwise false */
bool rc_is_exec (const char *pathname);

/*! Return a NULL terminted sorted list of the contents of the directory
 * @param dir to list
 * @param options any options to apply
 * @return NULL terminated list */
char **rc_ls_dir (const char *dir, int options);

/*! Remove a directory
 * @param pathname to remove
 * @param top remove the top level directory too
 * @return true if successful, otherwise false */
bool rc_rm_dir (const char *pathname, bool top);

/*! @name Configuration */
/*! Return a NULL terminated list of non comment lines from a file. */
char **rc_get_list (const char *file);
/*! Return a NULL terminated list of key=value lines from a file. */
char **rc_get_config (const char *file);
/*! Return the value of the entry from a key=value list. */
char *rc_get_config_entry (char **list, const char *entry);

/*! Return a NULL terminated string list of variables allowed through
 * from the current environemnt. */
char **rc_filter_env (void);
/*! Return a NULL terminated string list of enviroment variables made from
 * our configuration files. */
char **rc_make_env (void);

/*! @name String List functions
 * Handy functions for dealing with string arrays of char **.
 * It's safe to assume that any function here that uses char ** is a string
 * list that can be manipulated with the below functions. Every string list
 * should be released with a call to rc_strlist_free.*/
/*! Duplicate the item, add it to end of the list and return a pointer to it.
 * @param list to add the item too
 * @param item to add.
 * @return pointer to newly added item */
char *rc_strlist_add (char ***list, const char *item);
/*! If the item does not exist in the list, duplicate it, add it to the
 * list and then return a pointer to it.
 * @param list to add the item too
 * @param item to add.
 * @return pointer to newly added item */
char *rc_strlist_addu (char ***list, const char *item);
/*! Duplicate the item, add it to the list at the point based on locale and
 * then return a pointer to it.
 * @param list to add the item too
 * @param item to add.
 * @return pointer to newly added item */
char *rc_strlist_addsort (char ***list, const char *item);
/*! Duplicate the item, add it to the list at the point based on C locale and
 * then return a pointer to it.
 * @param list to add the item too
 * @param item to add.
 * @return pointer to newly added item */
char *rc_strlist_addsortc (char ***list, const char *item);
/*! If the item does not exist in the list, duplicate it, add it to the
 * list based on locale and then return a pointer to it.
 * @param list to add the item too
 * @param item to add.
 * @return pointer to newly added item */
char *rc_strlist_addsortu (char ***list, const char *item);
/*! Free the item and remove it from the list. Return 0 on success otherwise -1.
 * @param list to add the item too
 * @param item to add.
 * @return 0 on success, otherwise -1 */
int rc_strlist_delete (char ***list, const char *item);
/*! Moves the contents of list2 onto list1, so list2 is effectively emptied.
 * Returns a pointer to the last item on the new list.
 * @param list1 to append to
 * @param list2 to move from
 * @return pointer to the last item on the list */
char *rc_strlist_join (char ***list1, char **list2);
/*! Reverses the contents of the list.
 * @param list to reverse */
void rc_strlist_reverse (char **list);
/*! Frees each item on the list and the list itself.
 * @param list to free */
void rc_strlist_free (char **list);

#endif
