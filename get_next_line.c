/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:31:31 by abiru             #+#    #+#             */
/*   Updated: 2023/02/28 18:00:18 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}

int	nl_pos(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	return (i);
}

char	*get_line(char *tmp)
{
	char	*line;
	int		i;

	line = NULL;
	i = 0;
	if (*tmp == '\0')
		return (NULL);
	line = (char *)malloc(sizeof(char) * nl_pos(tmp) + 2);
	while (tmp[i] && tmp[i] != '\n')
	{
		line[i] = tmp[i];
		i++;
	}
	if (tmp[i] && tmp[i] == '\n')
		line[i++] = '\n';
	line[i] = 0;
	return (line);
}

char	*read_line(int fd, char *fd_table)
{
	int		char_read;
	char	*nl;
	char	*buffer;

	char_read = 1;
	nl = ft_strchr(fd_table, '\n');
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (!nl && char_read > 0)
	{
		char_read = read(fd, buffer, BUFFER_SIZE);
		if (char_read < 0)
		{
			free(buffer);
			return (0);
		}
		buffer[char_read] = 0;
		fd_table = join_free(fd_table, buffer);
		nl = ft_strchr(buffer, '\n');
	}
	free(buffer);
	return (fd_table);
}

char	*get_next_line(int fd)
{
	static char	*fd_table;
	char		*line;

	line = NULL;
	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0 || BUFFER_SIZE > 2147483647 || read(fd, 0, 0) == -1)
		return (NULL);
	if (!fd_table)
		fd_table = ft_strdup("");
	fd_table = read_line(fd, fd_table);
	if (!fd_table)
		return (NULL);
	line = get_line(fd_table);
	free(fd_table);
	fd_table = 0;
	return (line);
}
