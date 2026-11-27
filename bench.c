/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 15:10:00 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 13:10:45 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static long	*op_counts(void)
{
	static long	counts[OP_COUNT];

	return (counts);
}

void	bench_count(t_op op)
{
	op_counts()[op]++;
}

static void	put_nbr_err(long n)
{
	char	c;

	if (n >= 10)
		put_nbr_err(n / 10);
	c = '0' + n % 10;
	write(2, &c, 1);
}

static void	put_ops(void)
{
	static const char	*names[OP_COUNT] = {"sa", "sb", "ss", "pa", "pb",
		"ra", "rb", "rr", "rra", "rrb", "rrr"};
	long				total;
	int					i;

	total = 0;
	i = -1;
	while (++i < OP_COUNT)
		total += op_counts()[i];
	write(2, "[bench] total_ops: ", 19);
	put_nbr_err(total);
	i = -1;
	while (++i < OP_COUNT)
	{
		if (i == OP_SA || i == OP_RA)
			write(2, "\n[bench]", 8);
		write(2, " ", 1);
		write(2, names[i], 2 + (i >= OP_RRA));
		write(2, ": ", 2);
		put_nbr_err(op_counts()[i]);
	}
	write(2, "\n", 1);
}

void	bench_print(double disorder, const char *strategy)
{
	long	pct;
	int		len;

	pct = (long)(disorder * 10000.0 + 0.5);
	write(2, "[bench] disorder: ", 18);
	put_nbr_err(pct / 100);
	write(2, ".", 1);
	if (pct % 100 < 10)
		write(2, "0", 1);
	put_nbr_err(pct % 100);
	write(2, "%\n[bench] strategy: ", 20);
	len = 0;
	while (strategy[len])
		len++;
	write(2, strategy, len);
	write(2, "\n", 1);
	put_ops();
}
