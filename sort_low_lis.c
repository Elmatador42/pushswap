/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_low_lis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 15:00:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:48:12 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	*alloc_zero(int n)
{
	int	*arr;
	int	i;

	arr = malloc(sizeof(int) * n);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < n)
		arr[i++] = 0;
	return (arr);
}

static void	fill_seq(t_node *a, int *seq, int n)
{
	t_node	*cur;
	int		k;

	cur = a;
	while (cur->index != 0)
		cur = cur->next;
	k = 0;
	while (k < n)
	{
		seq[k++] = cur->index;
		cur = cur->next;
		if (!cur)
			cur = a;
	}
}

static int	lis_dp(int *seq, int n, int *len, int *prev)
{
	int	i;
	int	j;
	int	best;

	best = 0;
	i = -1;
	while (++i < n)
	{
		len[i] = 1;
		prev[i] = -1;
		j = -1;
		while (++j < i)
		{
			if (seq[j] < seq[i] && len[j] + 1 > len[i])
			{
				len[i] = len[j] + 1;
				prev[i] = j;
			}
		}
		if (len[i] > len[best])
			best = i;
	}
	return (best);
}

int	mark_lis(t_node *a, int n, int *keep)
{
	int	*seq;
	int	*len;
	int	*prev;
	int	best;
	int	failed;

	seq = malloc(sizeof(int) * n);
	len = malloc(sizeof(int) * n);
	prev = malloc(sizeof(int) * n);
	failed = (!seq || !len || !prev);
	if (!failed)
	{
		fill_seq(a, seq, n);
		best = lis_dp(seq, n, len, prev);
		while (best >= 0)
		{
			keep[seq[best]] = 1;
			best = prev[best];
		}
	}
	free(seq);
	free(len);
	free(prev);
	return (failed);
}
