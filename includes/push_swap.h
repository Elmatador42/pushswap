#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

/* operations, il faut encore les coder avec l'affichage et le compteur */
void		pa(t_node **a, t_node **b);
void		pb(t_node **a, t_node **b);
void		ra(t_node **a);
void		rb(t_node **b);
void		rrb(t_node **b);

/* strategies */
void		sort_simple(t_node **a, t_node **b);
void		sort_medium(t_node **a, t_node **b);
void		sort_complex(t_node **a, t_node **b);
const char	*sort_adaptive(t_node **a, t_node **b);

/* disorder */
double		compute_disorder(t_node *a);

/* utils */
int			stack_size(t_node *s);
void		assign_index(t_node *a);
int			ft_isqrt(int n);
int			find_index_pos(t_node *s, int index);

#endif
