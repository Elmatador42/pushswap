/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_low.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:00:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:48:22 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	apply_move(t_node **a, t_node **b, t_move m)
{
	while (m.ca > 0 && m.cb > 0)
	{
		rr(a, b);
		m.ca--;
		m.cb--;
	}
	while (m.ca < 0 && m.cb < 0)
	{
		rrr(a, b);
		m.ca++;
		m.cb++;
	}
	while (m.ca > 0 && m.ca--)
		ra(a);
	while (m.ca < 0 && m.ca++)
		rra(a);
	while (m.cb > 0 && m.cb--)
		rb(b);
	while (m.cb < 0 && m.cb++)
		rrb(b);
	pa(a, b);
}

static void	push_non_lis(t_node **a, t_node **b, int *keep, int n)
{
	int	todo;
	int	i;

	todo = 0;
	i = -1;
	while (++i < n)
		todo += !keep[i];
	while (todo > 0)
	{
		if (keep[(*a)->index])
			ra(a);
		else
		{
			pb(a, b);
			todo--;
		}
	}
}

static void	rotate_min_top(t_node **a)
{
	int	pos;
	int	size;

	size = stack_size(*a);
	pos = find_index_pos(*a, 0);
	if (pos <= size / 2)
		while (pos-- > 0)
			ra(a);
	else
		while (pos++ < size)
			rra(a);
}

static void	insert_back(t_node **a, t_node **b, t_low *ctx)
{
	while (*b)
	{
		low_refresh(*a, ctx);
		ctx->size_b = stack_size(*b);
		apply_move(a, b, low_cheapest(*b, ctx));
	}
}

void	sort_low(t_node **a, t_node **b)
{
	t_low	ctx;
	int		*keep;

	ctx.n = stack_size(*a);
	if (ctx.n < 2 || is_sorted(*a))
		return ;
	keep = alloc_zero(ctx.n);
	ctx.pos = malloc(sizeof(int) * ctx.n);
	ctx.next = malloc(sizeof(int) * (ctx.n + 1));
	if (!keep || !ctx.pos || !ctx.next || mark_lis(*a, ctx.n, keep))
		sort_simple(a, b);
	else
	{
		push_non_lis(a, b, keep, ctx.n);
		insert_back(a, b, &ctx);
		rotate_min_top(a);
	}
	free(keep);
	free(ctx.pos);
	free(ctx.next);
}
