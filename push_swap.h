/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 09:10:53 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 13:35:12 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <limits.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}					t_node;

typedef enum e_op
{
	OP_SA,
	OP_SB,
	OP_SS,
	OP_PA,
	OP_PB,
	OP_RA,
	OP_RB,
	OP_RR,
	OP_RRA,
	OP_RRB,
	OP_RRR,
	OP_COUNT
}					t_op;

typedef struct s_move
{
	int				ca;
	int				cb;
	int				total;
}					t_move;

typedef enum e_loc
{
	TOP_A,
	BOT_A,
	TOP_B,
	BOT_B
}					t_loc;

typedef struct s_chunk
{
	t_loc			loc;
	int				lo;
	int				size;
}					t_chunk;

typedef struct s_low
{
	int				*pos;
	int				*next;
	int				n;
	int				size_a;
	int				size_b;
}					t_low;

void				bench_count(t_op op);
void				bench_print(double disorder, const char *strategy);

void				sa(t_node **a);
void				sb(t_node **b);
void				ss(t_node **a, t_node **b);
void				pa(t_node **a, t_node **b);
void				pb(t_node **a, t_node **b);
void				ra(t_node **a);
void				rb(t_node **b);
void				rr(t_node **a, t_node **b);
void				rra(t_node **a);
void				rrb(t_node **b);
void				rrr(t_node **a, t_node **b);

void				sort_simple(t_node **a, t_node **b);
void				sort_small(t_node **a, t_node **b);
void				sort_medium(t_node **a, t_node **b);
void				sort_complex(t_node **a, t_node **b);
int					chunk_value(t_node *a, t_node *b, t_loc loc);
void				chunk_move(t_node **a, t_node **b, t_loc from, t_loc to);
void				chunk_base(t_node **a, t_node **b, t_chunk c);
void				sort_low(t_node **a, t_node **b);
int					mark_lis(t_node *a, int n, int *keep);
int					*alloc_zero(int n);
void				low_refresh(t_node *a, t_low *ctx);
t_move				low_cheapest(t_node *b, t_low *ctx);
const char			*sort_adaptive(t_node **a, t_node **b);
void				add_bench(t_node **a, t_node **b, int num);

double				compute_disorder(t_node *a);

int					stack_size(t_node *s);
void				assign_index(t_node *a);
int					ft_isqrt(int n);
int					find_index_pos(t_node *s, int index);
int					is_sorted(t_node *stack);
int					check_doubles(t_node **a);
int					chose_algo(int num, t_node **a, t_node **b);
int					parse_flags(char **av, int *i);
int					is_sep(char *str);
int					is_blank(char *str);

void				ft_lstadd_back(t_node **lst, t_node *new);
t_node				*ft_lstnew(int num);
void				ft_lstclear(t_node **lst);

#endif