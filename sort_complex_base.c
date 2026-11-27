/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_base.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:00:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:55:01 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	idx(t_node *a, int depth)
{
	while (depth-- > 0)
		a = a->next;
	return (a->index);
}

static void	sort_top_three(t_node **a)
{
	if (stack_size(*a) == 3)
	{
		if (idx(*a, 0) > idx(*a, 1) && idx(*a, 0) > idx(*a, 2))
			ra(a);
		else if (idx(*a, 1) > idx(*a, 2))
			rra(a);
	}
	else
	{
		if (idx(*a, 0) > idx(*a, 1) && idx(*a, 0) > idx(*a, 2))
			sa(a);
		if (idx(*a, 0) < idx(*a, 1) && idx(*a, 1) > idx(*a, 2))
		{
			ra(a);
			sa(a);
			rra(a);
		}
	}
	if (idx(*a, 0) > idx(*a, 1))
		sa(a);
}

void	chunk_base(t_node **a, t_node **b, t_chunk c)
{
	int	i;

	i = 0;
	if (c.loc != TOP_A)
		while (i++ < c.size)
			chunk_move(a, b, c.loc, TOP_A);
	if (c.size == 2 && idx(*a, 0) > idx(*a, 1))
		sa(a);
	else if (c.size == 3)
		sort_top_three(a);
}
