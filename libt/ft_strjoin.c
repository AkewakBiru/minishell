/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 13:02:31 by abiru             #+#    #+#             */
/*   Updated: 2022/10/02 13:22:40 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	int		s_len;
	int		ss_len;

	if (!s1)
		return (NULL);
	s_len = ft_strlen(s1);
	ss_len = ft_strlen(s2);
	ptr = malloc(sizeof(*s1) * (s_len + ss_len + 1));
	if (!ptr)
		return (NULL);
	s_len = 0;
	ss_len = 0;
	while (s1[s_len] != '\0')
	{
		ptr[s_len] = s1[s_len];
		s_len++;
	}
	while (s2[ss_len] != '\0')
		ptr[s_len++] = s2[ss_len++];
	ptr[s_len] = '\0';
	return (ptr);
}
