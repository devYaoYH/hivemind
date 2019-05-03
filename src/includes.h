#pragma once
//IO
#include<iostream>
#include<fstream>
#include<sstream>

//Data Structs
#include<string>
#include<vector>
#include<map>
#include<set>

//Util
#include<memory>
#include<algorithm>

//Process Control
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/select.h>
#include<sys/time.h>

//Macros
#define MAXLINE 1024
#define MAX_ARGS 128
#define STDIN 0
#define STDOUT 1
#define STDERR 2

//open() file creation flags (read/write | create | overwrite)
#define O_FLAGS O_RDWR | O_CREAT | O_TRUNC
//open() file permission flags (chmod 644)
#define S_FLAGS S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
