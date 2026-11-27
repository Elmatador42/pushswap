/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 12:26:29 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 14:55:04 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

const char	*sort_adaptive(t_node **a, t_node **b)
{
	double	disorder;
	int		small;

	disorder = compute_disorder(*a);
	small = (stack_size(*a) <= 5);
	if (small)
		sort_small(a, b);
	if (disorder < 0.2)
	{
		if (!small)
			sort_low(a, b);
		return ("Adaptive / O(n^2)");
	}
	if (disorder < 0.5)
	{
		if (!small)
			sort_medium(a, b);
		return ("Adaptive / O(n\xe2\x88\x9an)");
	}
	if (!small)
		sort_complex(a, b);
	return ("Adaptive / O(n log n)");
}
