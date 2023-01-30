/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 13:03:38 by abiru             #+#    #+#             */
/*   Updated: 2022/10/02 16:09:27 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_overload(char *ptr, char const *s)
{
	int	i;

	i = 0;
	ptr = (char *)malloc(sizeof(*s) * 1);
	if (!ptr)
		return (NULL);
	ptr[i] = 0;
	return (ptr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			i;
	char			*ptr;
	unsigned int	size;

	if (!s)
		return (NULL);
	i = 0;
	ptr = NULL;
	size = ft_strlen(s);
	if (start >= size)
		return ((char *)ft_overload(ptr, s));
	if (len > size - start)
		ptr = (char *)malloc(sizeof(*s) * (size - start + 1));
	else
		ptr = (char *)malloc(sizeof(*s) * (len + 1));
	if (!ptr)
		return (NULL);
	while (i < len && s[start])
		ptr[i++] = s[start++];
	ptr[i] = '\0';
	return (ptr);
}
