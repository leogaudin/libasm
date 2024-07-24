#ifndef LIBASM_H
#define LIBASM_H

#define NC "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BOLD "\033[1m"

#define BUFFER_SIZE 100

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int			ft_strlen(char *str);
char		*ft_strcpy(char *dst, char *src);
int			ft_strcmp(char *s1, char *s2);
ssize_t		ft_write(unsigned int fd, const char *str, size_t len);
ssize_t		ft_read(int fd, void *buf, size_t nbyte);
char		*ft_strdup(const char *s);

#endif
