/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfleskes <viktor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:35:11 by vfleskes          #+#    #+#             */
/*   Updated: 2026/11/26 18:29:10 by vfleskes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	check_doubles(t_node **a)
{
	t_node	*num;
	t_node	*comp;

	if (a == NULL)
		return (0);
	num = *a;
	while (num)
	{
		comp = num->next;
		while (comp)
		{
			if (num->value == comp->value)
				return (1);
			comp = comp->next;
		}
		num = num->next;
	}
	return (0);
}

int	chose_algo(int num, t_node **a, t_node **b)
{
	if (!(*a))
		return (write(2, "Error\n", 6), 1);
	if (num == 1)
		sort_simple(a, b);
	else if (num == 2)
		sort_medium(a, b);
	else if (num == 3)
		sort_complex(a, b);
	else if (num == 4 || num == 5)
		sort_adaptive(a, b);
	else
		add_bench(a, b, num);
	return (0);
}

void	add_bench(t_node **a, t_node **b, int num)
{
	double		disorder;
	const char	*strategy;

	disorder = compute_disorder(*a);
	strategy = "O(n log n)";
	if (num == 11)
	{
		sort_simple(a, b);
		strategy = "O(n^2)";
	}
	else if (num == 12)
	{
		sort_medium(a, b);
		strategy = "O(n\xe2\x88\x9an)";
	}
	else if (num == 13)
		sort_complex(a, b);
	else
		strategy = sort_adaptive(a, b);
	bench_print(disorder, strategy);
}

int	is_sep(char *str)
{
	int	i;

	i = 0;
	if (((str[i] == '-' || str[i] == '+') && (str[i + 1] >= '0'
				&& str[i + 1] <= '9')) || (str[i] >= '0' && str[i] <= '9'))
		return (1);
	return (0);
}

int	is_blank(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (str[i] == '\0');
}
