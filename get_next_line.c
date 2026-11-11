/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kisella <kisella@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 11:23:51 by kisella           #+#    #+#             */
/*   Updated: 2026/11/11 16:45:29 by kisella          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_to_stock(int fd, char *stock)
{
	char	*buffer;
	ssize_t	byte_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free(stock), NULL);
	while (!ft_strchr(stock, '\n'))
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			free(stock);
			free(buffer);
			return (NULL);
		}
		if (byte_read == 0)
			break ;
		buffer[byte_read] = '\0';
		stock = ft_strjoin(stock, buffer);
		if (!stock)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (stock);
}

static char	*extract_line(char *stock)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	j = 0;
	if (!stock || !stock[0])
		return (NULL);
	while (stock[i] != '\0' && stock[i] != '\n')
		i++;
	if (stock[i] == '\n')
		i += 1;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	while (j < i)
	{
		line[j] = stock[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
}
