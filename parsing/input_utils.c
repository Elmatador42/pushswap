/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfleskes <viktor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 16:35:11 by vfleskes          #+#    #+#             */
/*   Updated: 2026/11/26 17:13:37 by vfleskes         ###   ########.fr       */
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

void	chose_algo(int num, t_node **a, t_node **b)
{
	if (num == 1)
		sort_simple(a, b);
	else if (num == 2)
		sort_medium(a, b);
	else if (num == 3)
		sort_complex(a, b);
	else if (num == 5)
		bench(a, b);
	else
		sort_adaptive(a, b);
}

void	bench(t_node **a, t_node **b)
{
	sort_adaptive(a, b);
}
