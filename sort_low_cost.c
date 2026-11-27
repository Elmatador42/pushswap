/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_low_cost.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:00:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:48:03 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	low_refresh(t_node *a, t_low *ctx)
{
	int	i;

	i = -1;
	while (++i < ctx->n)
		ctx->pos[i] = -1;
	i = 0;
	while (a)
	{
		ctx->pos[a->index] = i++;
		a = a->next;
	}
	ctx->size_a = i;
	ctx->next[ctx->n] = -1;
	i = ctx->n;
	while (--i >= 0)
	{
		ctx->next[i] = ctx->next[i + 1];
		if (ctx->pos[i] >= 0)
			ctx->next[i] = i;
	}
}

static int	target_pos(t_low *ctx, int v)
{
	int	t;

	t = ctx->next[v + 1];
	if (t < 0)
		t = ctx->next[0];
	return (ctx->pos[t]);
}

static void	try_move(t_move *best, int ca, int cb)
{
	int	x;
	int	y;
	int	total;

	x = ca;
	if (x < 0)
		x = -x;
	y = cb;
	if (y < 0)
		y = -y;
	total = x + y;
	if ((ca >= 0) == (cb >= 0) && x > y)
		total = x;
	else if ((ca >= 0) == (cb >= 0))
		total = y;
	if (total < best->total)
	{
		best->ca = ca;
		best->cb = cb;
		best->total = total;
	}
}

static t_move	move_cost(int ja, int ib, int sa, int sb)
{
	t_move	best;

	best.total = INT_MAX;
	try_move(&best, ja, ib);
	try_move(&best, ja - sa, ib - sb);
	try_move(&best, ja, ib - sb);
	try_move(&best, ja - sa, ib);
	return (best);
}

t_move	low_cheapest(t_node *b, t_low *ctx)
{
	t_move	best;
	t_move	m;
	int		i;

	best.total = INT_MAX;
	i = 0;
	while (b)
	{
		m = move_cost(target_pos(ctx, b->index), i, ctx->size_a, ctx->size_b);
		if (m.total < best.total)
			best = m;
		b = b->next;
		i++;
	}
	return (best);
}
