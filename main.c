#include "libasm.h"

void		test_strlen(void)
{
	printf(BOLD "\nTesting ft_strlen\n" NC);

	char	*strings[] = {
		"test12345",
		"1",
		"test",
		"",
		"\n",
		"zzzzzzzzzzzzzzzzzzzzzzzz hey",
	};

	int	numStrings = sizeof(strings) / sizeof(strings[0]);

	for (int i = 0; i < numStrings; i++) {
		int	expected = strlen(strings[i]);
		int	result = ft_strlen(strings[i]);

		printf(expected == result ? GREEN "OK" NC : RED "KO" NC);
		printf("\t Expected: %d\tResult: %d\n", expected, result);
	}
}

void	test_strcpy(void)
{
	printf(BOLD "\nTesting ft_strcpy\n" NC);

	char	*strings[] = {
		"test12345",
		"1",
		"test",
		"",
		"zzzzzzzz",
	};

	int	numStrings = sizeof(strings) / sizeof(strings[0]);

	for (int i = 0; i < numStrings; i++) {
		// Test return value
		char	dummy_dest[BUFFER_SIZE];

		void	*ref_ptr = strcpy(dummy_dest, strings[i]);
		void	*test_ptr = ft_strcpy(dummy_dest, strings[i]);
		printf(ref_ptr == test_ptr ? GREEN "POINTER OK" NC : RED "POINTER KO" NC);
		printf("\tExpected: %p\tResult: %p\n", ref_ptr, test_ptr);

		// Test destination string
		char	ref_dest[BUFFER_SIZE];
		char	test_dest[BUFFER_SIZE];

		strcpy(ref_dest, strings[i]);
		ft_strcpy(test_dest, strings[i]);
		printf(strcmp(ref_dest, test_dest) == 0 ? GREEN "COPY OK" NC : RED "COPY KO" NC);
		printf("\t\tExpected: %s\tResult: %s\n", ref_dest, test_dest);
	}
}

void	test_strcmp(void)
{
	printf(BOLD "\nTesting ft_strcmp\n" NC);

	char	*strings1[] = {
		"test123456770797",
		"1",
		"test",
		"",
		"zzzzzzzz",
	};

	char	*strings2[] = {
		"test1234567",
		"2",
		"test2",
		"",
		"zzzz1zzz",
	};

	int	numStrings = sizeof(strings1) / sizeof(strings1[0]);

	for (int i = 0; i < numStrings; i++) {
		int	expected = strcmp(strings1[i], strings2[i]);
		int	result = ft_strcmp(strings1[i], strings2[i]);

		printf(BOLD "%s" NC " vs "BOLD "%s" NC"\n", strings1[i], strings2[i]);
		printf(expected == result ? GREEN "OK" NC : RED "KO" NC);
		printf("\t Expected: %d\tResult: %d\n\n", expected, result);
	}
}

void	test_write(void)
{
	printf(BOLD "\nTesting ft_write\n" NC);

	char	*strings[] = {
		"test123456",
		"1",
		"test",
		"",
		"\0",
		"zzzzzzzz",
	};


	int fds[] = {
		-1,
		open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644),
		10
	};

	int	numStrings = sizeof(strings) / sizeof(strings[0]);
	int	numFds = sizeof(fds) / sizeof(fds[0]);

	for (int i = 0; i < numStrings; i++) {
		// Test return value and errno
		printf(BOLD "\"%s\"\n" NC, strings[i]);
		for (int j = 0; j < numFds; j++) {
			errno = 0;
			int	ref_ret = write(fds[j], strings[i], strlen(strings[i]));
			int	ref_errno = errno;
			errno = 0;
			int	test_ret = ft_write(fds[j], strings[i], strlen(strings[i]));
			int	test_errno = errno;

			printf("fd: %d\t", fds[j]);
			printf(ref_ret == test_ret ? GREEN "RETURN OK" NC : RED "RETURN KO" NC);
			printf("\tExpected: %d\tResult: %d\n", ref_ret, test_ret);
			printf(ref_errno == test_errno ? GREEN "\tERRNO OK" NC : RED "\tERRNO KO" NC);
			printf("\tExpected: %d\tResult: %d\n", ref_errno, test_errno);
		}
		printf("\n");
	}
	// NULL pointer
	printf(BOLD "NULL string\n\n" NC);
	for (int j = 0; j < numFds; j++) {
		errno = 0;
		int	ref_ret = write(fds[j], NULL, BUFFER_SIZE);
		int	ref_errno = errno;
		errno = 0;
		int	test_ret = ft_write(fds[j], NULL, BUFFER_SIZE);
		int	test_errno = errno;

		printf("fd: %d\t", fds[j]);
		printf(ref_ret == test_ret ? GREEN "RETURN OK" NC : RED "RETURN KO" NC);
		printf("\tExpected: %d\tResult: %d\n", ref_ret, test_ret);
		printf(ref_errno == test_errno ? GREEN "\tERRNO OK" NC : RED "\tERRNO KO" NC);
		printf("\tExpected: %d\tResult: %d\n", ref_errno, test_errno);
	}
}

void	test_read(void)
{
	printf(BOLD "\nTesting ft_read\n" NC);

	int fds[] = {
		-1,
		open("read.txt", O_RDONLY, 0644),
		10
	};

	int	numFds = sizeof(fds) / sizeof(fds[0]);

	// Test return value and errno
	for (int j = 0; j < numFds; j++) {
		errno = 0;
		char	ref_buf[BUFFER_SIZE];
		int	ref_ret = read(fds[j], ref_buf, BUFFER_SIZE);
		ref_buf[ref_ret == -1 ? 0 : ref_ret] = '\0';
		int	ref_errno = errno;
		lseek(fds[j], 0, SEEK_SET);	// Reset file pointer
		errno = 0;
		char	test_buf[BUFFER_SIZE];
		int	test_ret = ft_read(fds[j], test_buf, BUFFER_SIZE);
		test_buf[test_ret == -1 ? 0 : test_ret] = '\0';
		int	test_errno = errno;

		printf("\nfd: %d\t", fds[j]);
		printf(ref_ret == test_ret ? GREEN "RETURN OK" NC : RED "RETURN KO" NC);
		printf("\tExpected: %d\tResult: %d\n\t", ref_ret, test_ret);
		printf(ref_errno == test_errno ? GREEN "ERRNO OK" NC : RED "ERRNO KO" NC);
		printf("\tExpected: %d\tResult: %d\n\t", ref_errno, test_errno);
		printf(strcmp(ref_buf, test_buf) == 0 ? GREEN "COPY OK" NC : RED "COPY KO" NC);
		printf("\tExpected: %s\tResult: %s\n", ref_buf, test_buf);
	}
	printf("\n");
}

int	main(void)
{
	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write();
	test_read();
	return (0);
}
