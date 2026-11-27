/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:00:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:55:38 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while ((str1[i] != '\0' || str2[i] != '\0'))
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

static int	flag_code(char *s)
{
	if (ft_strcmp(s, "--simple") == 0)
		return (1);
	if (ft_strcmp(s, "--medium") == 0)
		return (2);
	if (ft_strcmp(s, "--complex") == 0)
		return (3);
	if (ft_strcmp(s, "--adaptive") == 0)
		return (4);
	if (ft_strcmp(s, "--bench") == 0)
		return (10);
	return (-1);
}

int	parse_flags(char **av, int *i)
{
	int	bench;
	int	strat;
	int	code;

	bench = 0;
	strat = 0;
	while (av[*i] && av[*i][0] == '-' && av[*i][1] == '-')
	{
		code = flag_code(av[*i]);
		if (code < 0 || (code == 10 && bench) || (code < 10 && strat))
			return (-1);
		if (code == 10)
			bench = 10;
		else
			strat = code;
		(*i)++;
	}
	if (!strat)
		strat = 5;
	return (bench + strat);
}
