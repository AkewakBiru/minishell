/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 12:57:30 by abiru             #+#    #+#             */
/*   Updated: 2022/09/25 17:17:56 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*t1;
	unsigned char	*t2;

	i = 0;
	t1 = (unsigned char *)s1;
	t2 = (unsigned char *)s2;
	while (i < n)
	{
		if (*(t1 + i) > *(t2 + i))
			return (t1[i] - t2[i]);
		else if (*(t1 + i) < *(t2 + i))
			return (t1[i] - t2[i]);
		i++;
	}
	return (0);
}
