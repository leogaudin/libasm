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

int	main(void)
{
	test_strlen();
	test_strcpy();
	return (0);
}
