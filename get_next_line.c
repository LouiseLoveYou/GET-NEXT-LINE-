/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kisella <kisella@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 11:23:51 by kisella           #+#    #+#             */
/*   Updated: 2026/11/12 13:33:32 by kisella          ###   ########.fr       */
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

static char	*updated_stock(char *stock)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!stock)
		return (NULL);
	while (stock[i] && stock[i] != '\n')
		i++;
	if (!stock[i] || !stock[i + 1])
		return (free(stock), NULL);
	str = malloc(ft_strlen(stock) - i);
	if (!str)
		return (free(stock), NULL);
	i++;
	while (stock[i])
		str[j++] = stock[i++];
	str[j] = '\0';
	free(stock);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stock;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stock = read_to_stock(fd, stock);
	if (!stock)
		return (NULL);
	line = extract_line(stock);
	stock = updated_stock(stock);
	return (line);
}
