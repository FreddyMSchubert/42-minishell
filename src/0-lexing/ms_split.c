/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:40:24 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/15 12:57:32 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @brief counts how many pointers are needed in pointer array
int	count_tokens(const char *s)
{
	int	i;
	int	count;
	int	in_word;
	int	in_quote;

	i = -1;
	count = 0;
	in_word = 0;
	in_quote = 0;
	while ((int)ft_strlen(s) > i && s[++i])
	{
		if (is_operator_symbol(s[i], s[i + 1]))
		{
			// printf("operator: %c\n", s[i]);
			if (is_operator_symbol(s[i], s[i + 1]) == 2)
				i++;
			count++;
			in_word = 0;
		}
		else if (!ft_isspace(s[i]) && !in_word && !in_quote) //count new word
		{
			count++;
			in_word = 1;
			// printf("wortanfang: %c\n", s[i]);
		}
		else if (ft_isspace(s[i]) && !in_quote)
			in_word = 0;
	}
	if (in_quote)
		return (-1);
	return (count);
}

/// @brief gets start point of string to be split,
/// determines end point by the next delim in s
/// @return splitted string
static char	*make_split_str(const char *s, char delim, int *i)
{
	size_t	size;

	size = *i;
	if (delim == '\'' || delim == '"')
		*i += 1;
	while (s[*i] && s[*i] != delim)
		*i += 1;
	if (delim != '\'' && delim != '"')
		*i -= 1;
	return (ft_substr(s, size, *i - size + 1));
}

/// @brief frees the 2d array in case of a allocation fail
void	free_split(char **split)
{
	int	i;

	i = 0;
	if (split == NULL)
		return ;
	while (split[i])
	{
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	free(split);
	split = NULL;
}

/// @brief splits a string into an array of strings, using spaces,
/// single quotes and double quotes as delimiters. inside single
/// quotes: everything is a string
char	**ms_split(char *input)
{
	int				word_count;
	char			**result;
	int				i;
	int				j;

	i = -1;
	j = 0;
	word_count = count_tokens(input);
	if (word_count == -1)
		return (NULL);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	result[word_count] = NULL;
	while (input[++i] && j < word_count)
	{
		if (is_operator_symbol(input[i], input[i + 1]))
		{
			result[j++] = make_split_str(input, ' ', &i);
			if (!result[j - 1])
				return (free_split(result), NULL);
		}
		else if (!ft_isspace(input[i]))
		{
			result[j++] = make_split_str(input, ' ', &i);
			if (!result[j - 1])
				return (free_split(result), NULL);
		}
	}
	return (result);
}


// int main(void)
// {
// 	char *input1 = ">>";
// 	// char *input1 = "( echo \"Hello $USER\" && ( export | ) cat < input.txt > output.txt ) ) || ( echo $? && ls * && cd /home ) && echo \"Nested start\" && ( cd /tmp && ls ) && echo \"Nested end\"";;
// 	char **split = ms_split(input1);
// 	for (int i = 0; split && split[i]; i++)
// 	{
// 		printf("ms_split: .%s.\n", split[i]);
// 	}
// 	// printf("word_count: %d\n", count_words(argv[1]));
// 	// printf("word_count: %d\n", count_words(input1));
// 	return 0;
// }

/*
different operator symbols:
< > | & ( )
<< >> || && -> not implemented
*/
