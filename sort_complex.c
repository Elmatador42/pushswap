/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 13:28:57 by vfleskes          #+#    #+#             */
/*   Updated: 2026/11/27 14:54:45 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_loc	normalize(t_node *a, t_node *b, t_chunk c)
{
	if (c.loc == BOT_A && stack_size(a) == c.size)
		return (TOP_A);
	if (c.loc == BOT_B && stack_size(b) == c.size)
		return (TOP_B);
	return (c.loc);
}

static void	set_dest(t_loc loc, t_chunk *out)
{
	out[2].loc = TOP_A;
	if (loc == TOP_A)
		out[2].loc = BOT_A;
	out[1].loc = BOT_A;
	if (loc == TOP_A || loc == BOT_A)
		out[1].loc = TOP_B;
	out[0].loc = BOT_B;
	if (loc == BOT_B)
		out[0].loc = TOP_B;
}

static void	split(t_node **a, t_node **b, t_chunk c, t_chunk *out)
{
	int	i;
	int	v;

	out[0].lo = c.lo;
	out[1].lo = c.lo + c.size / 4;
	out[2].lo = c.lo + c.size * 3 / 5;
	out[0].size = out[1].lo - out[0].lo;
	out[1].size = out[2].lo - out[1].lo;
	out[2].size = c.lo + c.size - out[2].lo;
	set_dest(c.loc, out);
	i = 0;
	while (i++ < c.size)
	{
		v = chunk_value(*a, *b, c.loc);
		if (v >= out[2].lo)
			chunk_move(a, b, c.loc, out[2].loc);
		else if (v >= out[1].lo)
			chunk_move(a, b, c.loc, out[1].loc);
		else
			chunk_move(a, b, c.loc, out[0].loc);
	}
}

static void	rec_sort(t_node **a, t_node **b, t_chunk c)
{
	t_chunk	out[3];

	c.loc = normalize(*a, *b, c);
	if (c.size <= 3)
	{
		chunk_base(a, b, c);
		return ;
	}
	split(a, b, c, out);
	rec_sort(a, b, out[2]);
	rec_sort(a, b, out[1]);
	rec_sort(a, b, out[0]);
}

void	sort_complex(t_node **a, t_node **b)
{
	t_chunk	c;

	if (is_sorted(*a))
		return ;
	c.loc = TOP_A;
	c.lo = 0;
	c.size = stack_size(*a);
	rec_sort(a, b, c);
}
