#ifndef __IO_MANAGER_H__
#define __IO_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>

#include "limits.h"
#include "file_types.h"

using namespace std;

class IO_Manager {

  public:
    IO_Manager();

    /*
     *	Translates a read request from the stripe level to the chunk level.
     *	The correct behavior of this function depends on the
     *	Distribution and Replication strategies that are in place.
     */
    ssize_t process_read_stripe (uint32_t file_id, char *pathname,
                                 uint32_t stripe_id, void *buf,
                                 size_t count);

    /*
     *	Translates a write request into a series of chunk writes and handles
     *	replication. 
     *	The correct behavior of this function depends on the
     *	Distribution and Replication strategies that are in place.
     */
    ssize_t process_write_stripe (uint32_t file_id, char *pathname,
                                  uint32_t stripe_id, void *buf,
                                  size_t count);

    /*
     *	Set the storage location (node id) for a given chunk of a file.
     */
    int set_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                     uint32_t node_id);

    /*
     *	Get the storage location (node id) for a given chunk of a file.
     */
    int get_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

    /*
     *	Set the storage location (node id) for a given replica of a
     *	chunk of a file.
     */
    int set_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num, uint32_t node_id);

    /*
     *	Get the storage location (node id) for a given replica of a
     *	chunk of a file.
     */
    int get_replica_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

    /*
     *	Fill in struct decafs_file_stat structure that provides information
     *	about where the chunks live for a specific file.
     */
    int stat_file_name (char *pathname, struct decafs_file_stat *buf);
    int stat_file_id (uint32_t file_id, struct decafs_file_stat *buf);

    /*
     *	Fill in struct decafs_file_stat structure that provides information
     *	about where the stripes live for a specific file.
     */
    int stat_replica_name (char *pathname, struct decafs_file_stat *buf);
    int stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf);
};
    
/*
 *	Translates a read request from the stripe level to the chunk level.
 *	The correct behavior of this function depends on the
 *	Distribution and Replication strategies that are in place.
 */
extern "C" ssize_t process_read_stripe (uint32_t file_id, char *pathname,
                                        uint32_t stripe_id, void *buf,
                                        size_t count);


/*
 *	Translates a write request into a series of chunk writes and handles
 *	replication. 
 *	The correct behavior of this function depends on the
 *	Distribution and Replication strategies that are in place.
 */
extern "C" ssize_t process_write_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, void *buf,
                                         size_t count);

/*
 *	Set the storage location (node id) for a given chunk of a file.
 */
extern "C" int set_node_id (uint32_t file_id, uint32_t stripe_id,
                            uint32_t chunk_num, uint32_t node_id);

/*
 *	Get the storage location (node id) for a given chunk of a file.
 */
extern "C" int get_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num);

/*
 *	Set the storage location (node id) for a given replica of a
 *	chunk of a file.
 */
extern "C" int set_replica_node_id (uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
                                    uint32_t node_id);

/*
 *	Get the storage location (node id) for a given replica of a
 *	chunk of a file.
 */
extern "C" int get_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                    uint32_t chunk_num);

/*
 *	Fill in struct decafs_file_stat structure that provides information
 *	about where the chunks live for a specific file.
 */
extern "C" int stat_file_name (char *pathname, struct decafs_file_stat *buf);
extern "C" int stat_file_id (uint32_t file_id, struct decafs_file_stat *buf);

/*
 *	Fill in struct decafs_file_stat structure that provides information
 *	about where the stripes live for a specific file.
 */
extern "C" int stat_replica_name (char *pathname, struct decafs_file_stat *buf);
extern "C" int stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf);

#endif
