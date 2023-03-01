/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 19:48:32 by abiru             #+#    #+#             */
/*   Updated: 2023/03/01 18:55:45 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	skip_spaces(const char *str)
{
	static int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

static int	cus_strlen(const char *str)
{
	int	i;
	int	j;

	j = 0;
	i = skip_spaces(str);
	while (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] == '0')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
			j++;
		else
			break ;
		i++;
	}
	return (j);
}

static int	ft_sign(const char *str, int i)
{
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			return (-1);
	}
	return (1);
}

unsigned long long	ft_atoi(const char *str)
{
	unsigned long long	result;
	int					i;
	int					sign;

	result = 0;
	sign = 1;
	i = (int)skip_spaces(str);
	sign = (int)ft_sign(str, i);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if ((str + i && str[i] == 0) || (cus_strlen(str) >= 20 && sign == 1))
		return (9223372036854775809ULL);
	if (cus_strlen(str) >= 20 && sign == -1)
		return (9223372036854775809ULL);
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + str[i] - '0';
		i++;
	}
	if (result > 9223372036854775807 && sign == 1)
		return (9223372036854775809ULL);
	if (result > 9223372036854775808ULL && sign == -1)
		return (9223372036854775809ULL);
	return ((result * sign) % 256);
}
