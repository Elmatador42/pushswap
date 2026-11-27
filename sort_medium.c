/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 14:56:32 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 13:31:32 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	chunk_size(int n)
{
	int	chunk;

	chunk = ft_isqrt(n) * 3 / 2;
	if (chunk < 1)
		chunk = 1;
	return (chunk);
}

static void	push_chunks(t_node **a, t_node **b, int chunk)
{
	int	pushed;

	pushed = 0;
	while (*a)
	{
		if ((*a)->index <= pushed)
		{
			pb(a, b);
			if ((*b)->next)
				rb(b);
			pushed++;
		}
		else if ((*a)->index <= pushed + chunk)
		{
			pb(a, b);
			pushed++;
		}
		else
			ra(a);
	}
}

static void	push_back(t_node **a, t_node **b)
{
	int	size;
	int	pos;

	size = stack_size(*b);
	while (size > 0)
	{
		pos = find_index_pos(*b, size - 1);
		if (pos <= size / 2)
		{
			while (pos-- > 0)
				rb(b);
		}
		else
		{
			while (pos++ < size)
				rrb(b);
		}
		pa(a, b);
		size--;
	}
}

void	sort_medium(t_node **a, t_node **b)
{
	int	n;

	n = stack_size(*a);
	if (n < 2 || compute_disorder(*a) == 0.0)
		return ;
	if (n == 2)
		return (sa(a));
	push_chunks(a, b, chunk_size(n));
	push_back(a, b);
}
