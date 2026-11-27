/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 10:38:49 by vfleskes          #+#    #+#             */
/*   Updated: 2026/11/27 13:31:53 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_simple(t_node **a, t_node **b)
{
	int	size;
	int	pos;
	int	index;

	size = stack_size(*a);
	index = 0;
	if (is_sorted(*a))
		return ;
	while (size > 0)
	{
		pos = find_index_pos(*a, index);
		if (pos <= size / 2)
			while ((*a)->index != index)
				ra(a);
		else
			while ((*a)->index != index)
				rra(a);
		pb(a, b);
		index++;
		size--;
	}
	while (*b)
		pa(a, b);
}
