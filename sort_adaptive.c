/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 12:26:29 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 13:31:15 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

const char	*sort_adaptive(t_node **a, t_node **b)
{
	double	disorder;

	disorder = compute_disorder(*a);
	if (disorder < 0.2)
	{
		sort_simple(a, b);
		return ("O(n^2)");
	}
	if (disorder < 0.5)
	{
		sort_medium(a, b);
		return ("O(n\xe2\x88\x9an)");
	}
	sort_complex(a, b);
	return ("O(n log n)");
}
