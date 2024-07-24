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
		char	dummy_dest[100];

		void	*ref_ptr = strcpy(dummy_dest, strings[i]);
		void	*test_ptr = ft_strcpy(dummy_dest, strings[i]);
		printf(ref_ptr == test_ptr ? GREEN "POINTER OK" NC : RED "POINTER KO" NC);
		printf("\tExpected: %p\tResult: %p\n", ref_ptr, test_ptr);

		// Test destination string
		char	ref_dest[100];
		char	test_dest[100];

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
		"test123456\n",
		"1\n",
		"test\n",
		"",
		"\0",
		"zzzzzzzz\n",
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
		printf(BOLD "String: %s\n" NC, strings[i]);
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
		int	ref_ret = write(fds[j], NULL, 100);
		int	ref_errno = errno;
		errno = 0;
		int	test_ret = ft_write(fds[j], NULL, 100);
		int	test_errno = errno;

		printf("fd: %d\t", fds[j]);
		printf(ref_ret == test_ret ? GREEN "RETURN OK" NC : RED "RETURN KO" NC);
		printf("\tExpected: %d\tResult: %d\n", ref_ret, test_ret);
		printf(ref_errno == test_errno ? GREEN "\tERRNO OK" NC : RED "\tERRNO KO" NC);
		printf("\tExpected: %d\tResult: %d\n", ref_errno, test_errno);
	}
}

int	main(void)
{
	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write();
	return (0);
}
