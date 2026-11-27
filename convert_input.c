/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfleskes <viktor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 12:28:49 by vfleskes          #+#    #+#             */
/*   Updated: 2026/11/26 18:51:48 by vfleskes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static long long	ft_atoi(const char *nptr)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10;
		res = res + (nptr[i] - '0');
		if (res > (long long)INT_MAX + 1)
			return (res * sign);
		i++;
	}
	return (res * sign);
}

static int	add_num(char *str, t_node **a)
{
	int (i) = 0;
	while (str[i])
	{
		while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (is_sep(&str[i]) == 1)
		{
			long long (num) = ft_atoi(&str[i]);
			if (num > INT_MAX || num < INT_MIN)
				return (ft_lstclear(a), 1);
			t_node (*newnode) = ft_lstnew(num);
			if (newnode == NULL)
				return (ft_lstclear(a), 1);
			ft_lstadd_back(a, newnode);
			if (str[i] == '-' || str[i] == '+')
				i++;
			while (str[i] >= '0' && str[i] <= '9')
				i++;
		}
		if (!(str[i] == ' ' || str[i] == '\0' || (str[i] >= 9 && str[i] <= 13)))
			return (ft_lstclear(a), 1);
	}
	if (is_blank(str))
		return (ft_lstclear(a), 1);
	return (0);
}

int	main(int ac, char **av)
{
	t_node	*a;
	t_node	*b;
	int		i;
	int		num;

	a = NULL;
	b = NULL;
	i = 1;
	if (ac > 1)
	{
		num = parse_flags(av, &i);
		if (num < 0)
			return (write(2, "Error\n", 6), 1);
		while (i < ac)
			if (add_num(av[i++], &a) == 1)
				return (write(2, "Error\n", 6), 1);
		if (!a)
			return (0);
		assign_index(a);
		if (check_doubles(&a) == 1)
			return (ft_lstclear(&a), write(2, "Error\n", 6), 1);
		if (chose_algo(num, &a, &b) == 1)
			return (1);
	}
	return (ft_lstclear(&a), ft_lstclear(&b), 0);
}
