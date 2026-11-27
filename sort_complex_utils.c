/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:00:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:54:53 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	bottom_index(t_node *s)
{
	while (s->next)
		s = s->next;
	return (s->index);
}

int	chunk_value(t_node *a, t_node *b, t_loc loc)
{
	if (loc == TOP_A)
		return (a->index);
	if (loc == BOT_A)
		return (bottom_index(a));
	if (loc == TOP_B)
		return (b->index);
	return (bottom_index(b));
}

void	chunk_move(t_node **a, t_node **b, t_loc from, t_loc to)
{
	int	in_a;

	if (from == BOT_A)
		rra(a);
	else if (from == BOT_B)
		rrb(b);
	in_a = (from == TOP_A || from == BOT_A);
	if (in_a && (to == TOP_B || to == BOT_B))
		pb(a, b);
	else if (!in_a && (to == TOP_A || to == BOT_A))
		pa(a, b);
	if (to == BOT_A)
		ra(a);
	else if (to == BOT_B)
		rb(b);
}
