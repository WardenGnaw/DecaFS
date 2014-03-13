#include "../../../src/app/barista_core/persistent_metadata.h"
#include "gtest/gtest.h"

#define STRIPE_SIZE 1024
#define CHUNK_SIZE 512
#define REPLICA_SIZE 512

const char *file_1 = "file 1";
const char *file_2 = "file 2";

const int file_1_id = 1;
const int file_2_id = 2;

struct file_instance inst = {1, 1, 1, 1};

TEST (Persistent_Metadata, AddFile) {
  Persistent_Metadata p_meta;
  struct timeval time;
  char *bad_file = (char *)malloc (MAX_FILENAME_LENGTH + 2);
  memset (bad_file, 'a', MAX_FILENAME_LENGTH + 1);
  bad_file[MAX_FILENAME_LENGTH + 1] = '\0';

  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (FILE_EXISTS, p_meta.add_file ((char *)file_1, file_1_id,
                                               STRIPE_SIZE, CHUNK_SIZE,
                                               REPLICA_SIZE, time));
  EXPECT_EQ (FILENAME_INVALID, p_meta.add_file ((char *)bad_file, file_1_id,
                                                    STRIPE_SIZE, CHUNK_SIZE,
                                                    REPLICA_SIZE, time));
}

TEST (Persistent_Metadata, GetNumFiles) {
  Persistent_Metadata p_meta;
  struct timeval time;
  
  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (1, p_meta.get_num_files());
  EXPECT_EQ (0, p_meta.add_file ((char *)file_2, file_2_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (2, p_meta.get_num_files());
}

TEST (Persistent_Metadata, Filenames) {
  Persistent_Metadata p_meta;
  struct timeval time;
  
  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (0, p_meta.add_file ((char *)file_2, file_2_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  
  char **filenames = (char **)malloc(2);
  filenames[0] = (char *)malloc (MAX_FILENAME_LENGTH);
  filenames[1] = (char *)malloc (MAX_FILENAME_LENGTH);

  EXPECT_EQ (2, p_meta.get_filenames(filenames, 2));
  ASSERT_STREQ (file_1, filenames[0]);
  ASSERT_STREQ (file_2, filenames[1]);

  free (filenames[0]);
  free (filenames[1]);
  free (filenames);
}

TEST (Persistent_Metadata, decafs_file_stat) {
  Persistent_Metadata p_meta;
  struct timeval time;
  struct decafs_file_stat stat;

  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (FILE_NOT_FOUND, p_meta.decafs_file_stat((char *)"junk", &stat));
  EXPECT_EQ (0, p_meta.decafs_file_stat((char *)file_1, &stat));
  EXPECT_EQ (file_1_id, stat.file_id);
  EXPECT_EQ (0, stat.size);
  EXPECT_EQ (STRIPE_SIZE, stat.stripe_size);
  EXPECT_EQ (CHUNK_SIZE, stat.chunk_size);
  EXPECT_EQ (REPLICA_SIZE, stat.replica_size);
}

TEST (Persistent_Metadata, set_access_time) {
  Persistent_Metadata p_meta;
  struct timeval time;
  struct decafs_file_stat stat;
  
  time.tv_sec = 0;
  time.tv_usec = 0;

  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (0, p_meta.decafs_file_stat((char *)file_1, &stat));
  EXPECT_EQ (0, stat.last_access_time.tv_sec); 
  EXPECT_EQ (0, stat.last_access_time.tv_usec);

  time.tv_sec = 100;
  time.tv_usec = 100;

  EXPECT_EQ (0, p_meta.set_access_time (inst, time));
  EXPECT_EQ (0, p_meta.decafs_file_stat((char *)file_1, &stat));
  EXPECT_EQ (100, stat.last_access_time.tv_sec); 
  EXPECT_EQ (100, stat.last_access_time.tv_usec);
}

TEST (Persistent_Metadata, update_file_size) {
  Persistent_Metadata p_meta;
  struct timeval time;
  struct decafs_file_stat stat;
  
  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (FILE_NOT_FOUND, p_meta.update_file_size (file_2_id, 100));
  EXPECT_EQ (100, p_meta.update_file_size (file_1_id, 100));
  EXPECT_EQ (0, p_meta.decafs_file_stat((char *)file_1, &stat));
  EXPECT_EQ (100, stat.size); 
}

