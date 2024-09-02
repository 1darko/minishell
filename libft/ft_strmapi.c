/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:51:53 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/09 13:51:54 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*function) (unsigned int, char))
{
	char	*new;
	size_t	cur;

	cur = 0;
	if (!s || !function)
		return (NULL);
	new = ft_strdup(s);
	if (!new)
		return (NULL);
	while (new[cur] != '\0')
	{
		new[cur] = function(cur, new[cur]);
		cur++;
	}
	return (new);
}
// int main() {
//     // Define a sample string
//     const char *input_str = "Hello, World!";

//     // Apply the function to the string
//     char *result_str = ft_strmapi(input_str, convert_to_upper);

//     // Check if memory allocation failed
//     if (!result_str) {
//         printf("Memory allocation failed.\n");
//         return 1; // Exit with an error code
//     }

//     // Print the modified string
//     printf("Modified string: %s\n", result_str);

//     // Free the dynamically allocated memory
//     free(result_str);

//     return 0;
// }
