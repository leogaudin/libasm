#include "libasm.h"

void		test_strlen(void)
{
	printf("\nTesting ft_strlen\n");

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

		if (expected == result) {
			printf(GREEN "OK" NC);
		} else {
			printf(RED "KO" NC);
		}
		printf("\t Expected: %d\tResult: %d\n", expected, result);
	}
}

int	main(void)
{
	test_strlen();
	return (0);
}
