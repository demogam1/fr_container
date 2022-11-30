/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaev <misaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:34:05 by misaev            #+#    #+#             */
/*   Updated: 2022/11/30 21:31:56 by misaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "bst_tri.hpp"
#include <functional>
#include <memory>
#include "bidirectional_iterator.hpp"
#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"
#include "equal.hpp"
#include "is_integral.hpp"
#include "lexicographical_compare.hpp"
#include "enable_if.hpp"
#include "pair.hpp"
#include "node.hpp"
#include "stack.hpp"

namespace	ft
{
	template < class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator< ft::pair <const Key, T> > >
	class	map
	{
		public:

	 		typedef Key	key_type;
			typedef T	mapped_type;
			typedef ft::pair<const key_type, mapped_type>	value_type;
			typedef Compare	key_compare;
			class value_compare : std::binary_function<value_type, value_type, bool>
			{
				friend class map;
				protected:
					Compare comp;
					value_compare (Compare c) : comp(c) {}
				public:
					bool operator() (const value_type& x, const value_type& y) const
  						{ return comp(x.first, y.first); }
			};
			typedef Alloc	allocator_type;		
			typedef typename allocator_type::reference	reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer	pointer;
			typedef typename allocator_type::const_pointer	const_pointer;
			typedef ft::bidirectional_iterator<value_type>	iterator;
			typedef ft::bidirectional_iterator<const value_type>	const_iterator;
			typedef ft::reverse_iterator<iterator>	reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
			typedef typename allocator_type::difference_type	difference_type;
			typedef typename allocator_type::size_type	size_type;

		private :

			key_compare	compare;
			allocator_type	alloc;
			size_type	_size;

			typedef Node<const value_type>	* const_Node;
			typedef Node<value_type>	Node;
			typedef	Node	* NodePtr;
			std::allocator<Node>	allocNode;
			NodePtr	root;
			NodePtr	TNULL;	

		public :

			explicit map(const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type()) : compare(comp), alloc(alloc), _size(0)
			{
				TNULL = allocNode.allocate(1);
				TNULL->parent = nullptr;
				TNULL->data = this->alloc.allocate(1);
				TNULL->color = 0;
				TNULL->left = nullptr;
				TNULL->right = nullptr;
				root = TNULL;
			}

			template <class InputIterator>
			map(InputIterator first, InputIterator last,
				const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, bool>::type = false)
				: compare(comp), alloc(alloc), _size(0)
			{
				TNULL = allocNode.allocate(1);
				TNULL->parent = nullptr;
				TNULL->data = this->alloc.allocate(1);
				TNULL->color = 0;
				TNULL->left = nullptr;
				TNULL->right = nullptr;
				root = TNULL;
				insert(first, last);
			}

			map(const map & x) : compare(x.compare), alloc(x.alloc), _size(0)
			{
				TNULL = allocNode.allocate(1);
				TNULL->parent = nullptr;
				TNULL->data = this->alloc.allocate(1);
				TNULL->color = 0;
				TNULL->left = nullptr;
				TNULL->right = nullptr;
				root = TNULL;
				*this = x;
			}

			map const	& operator=(const map & rhs)
			{
				clear();
				if (this != &rhs)
					insert(rhs.begin(), rhs.end());
				return (*this);
			}

			virtual ~map()
			{
				clear();
				alloc.destroy(TNULL->data);
				alloc.deallocate(TNULL->data, 1);
				allocNode.deallocate(TNULL, 1);
			}


			iterator	begin() { return (iterator(minimum(root), maximum(root), TNULL)); }
			const_iterator	begin() const
				{ return (const_iterator(reinterpret_cast<const_Node>(minimum(root)), reinterpret_cast<const_Node>(maximum(root)), reinterpret_cast<const_Node>(TNULL))); }

			iterator	end() { return (iterator(TNULL, maximum(root), TNULL)); }
			const_iterator	end() const
				{ return (const_iterator(reinterpret_cast<const_Node>(TNULL), reinterpret_cast<const_Node>(maximum(root)), reinterpret_cast<const_Node>(TNULL))); }

			reverse_iterator	rbegin() { return (reverse_iterator(end())); }
			const_reverse_iterator	rbegin() const  { return (const_reverse_iterator(end())); }

			reverse_iterator	rend() { return (reverse_iterator(begin())); }
			const_reverse_iterator	rend() const  { return (const_reverse_iterator(begin())); }


			bool	empty() const { return (_size == 0); }

			size_type	size() const { return (_size); }

			size_type	max_size() const { return (allocNode.max_size()); }


			mapped_type	& operator[](const key_type & k) { return ((*((this->insert(ft::make_pair(k,mapped_type()))).first)).second); }


			ft::pair<iterator, bool>	insert(const value_type & val)
			{
				NodePtr	node;
				if ((node = searchTree(root, val.first)) != TNULL)
					return (ft::pair<iterator, bool>(iterator(node, maximum(root), TNULL), false));


				node = allocNode.allocate(1);
				node->parent = nullptr;
				node->data = this->alloc.allocate(1);
				alloc.construct(node->data, value_type(val.first, val.second));
				node->left = TNULL;
				node->right = TNULL;
				node->color = 1;

				NodePtr	y = nullptr;
				NodePtr	x = this->root;

				while (x != TNULL)
				{
					y = x;
					if (compare(node->data->first, x->data->first))
						x = x->left;
					else
						x = x->right;
				}

				node->parent = y;
				if (y == nullptr)
					root = node;
				else if (  compare(node->data->first, y->data->first))
					y->left = node;
				else
					y->right = node;

				_size++;
				if (node->parent == nullptr)
				{
					node->color = 0;
					return (ft::pair<iterator, bool>(iterator(node, maximum(root), TNULL), true));
				}

				if (node->parent->parent == nullptr)
					return (ft::pair<iterator, bool>(iterator(node, maximum(root), TNULL), true));

				insertFix(node);
				return (ft::pair<iterator, bool>(iterator(searchTree(root, val.first), maximum(root), TNULL), true));
			}

			iterator	insert(iterator position, const value_type & val)
			{
				(void)position;
				return (insert(val).first);
			}

			template <class InputIterator>
			void	insert(InputIterator first, InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, bool>::type = false)
			{
				while (first != last)
					insert(*first++);
			}

			void	erase(iterator position)
			{
				erase(position->first);
			}

			size_type	erase(const key_type & key)
			{
				NodePtr	node = this->root;
				NodePtr	z = TNULL;
				NodePtr	x;
				NodePtr	y;
				while (node != TNULL)
				{
					if (node->data->first == key)
						z = node;
					if (compare(key, node->data->first))
						node = node->left;
					else
						node = node->right;
				}

				if (z == TNULL)
					return(0);

				// normal BST deletion
				y = z;
				int	y_original_color = y->color;
				if (z->left == TNULL)
				{
					x = z->right;
					rbTransplant(z, z->right);
				}
				else if (z->right == TNULL)
				{
					x = z->left;
					rbTransplant(z, z->left);
				}
				else
				{
					y = minimum(z->right);
					y_original_color = y->color;
					x = y->right;
					if (y->parent == z)
						x->parent = y;
					else
					{
						rbTransplant(y, y->right);
						y->right = z->right;
						y->right->parent = y;
					}

					rbTransplant(z, y);
					y->left = z->left;
					y->left->parent = y;
					y->color = z->color;
				}
				alloc.destroy(z->data);
				alloc.deallocate(z->data, 1);
				allocNode.deallocate(z, 1);
				if (y_original_color == 0)
					deleteFix(x);
				_size--;
				return (1);
			}

			void	erase(iterator first, iterator last)
			{
				while (first != last)
					erase(first++);
			}

			void	swap(map & x)
			{
				size_type	tmp_size = x._size;
				NodePtr	tmp_root = x.root;
				NodePtr	tmp_TNULL = x.TNULL;

				x._size = _size;
				x.root = root;
				x.TNULL = TNULL;

				_size = tmp_size;
				root = tmp_root;
				TNULL = tmp_TNULL;
			} 

			void	clear() { erase(begin(), end()); }

			/*	Observers	*/

			key_compare	key_comp() const { return (key_compare()); }

			value_compare	value_comp() const { return (value_compare(key_compare())); }			

			/*	Operations	*/

			iterator	find(const key_type & k) { return (iterator(searchTree(root, k), maximum(root), TNULL));}
			const_iterator	find(const key_type & k) const
				{ return (const_iterator(reinterpret_cast<const_Node>(searchTree(root, k)), reinterpret_cast<const_Node>(maximum(root)), reinterpret_cast<const_Node>(TNULL))); }

			size_type	count(const key_type & k) const { return ((searchTree(root, k) == TNULL) ? 0 : 1); }

			iterator	lower_bound(const key_type & k)
			{
				iterator	it;

				for (it = begin(); it != end(); it++)
					if (!compare(it->first, k))
						return (it);
				return (it);
			}
			const_iterator	lower_bound(const key_type & k) const
			{
				const_iterator	it;

				for (it = begin(); it != end(); it++)
					if (!compare(it->first, k))
						return (it);
				return (it);
			}
			
			iterator	upper_bound(const key_type & k)
			{
				iterator	it;

				for (it = begin(); it != end(); it++)
					if (compare(k, it->first))
						return (it);
				return (it);
			}
			const_iterator	upper_bound(const key_type & k) const
			{
				const_iterator	it;

				for (it = begin(); it != end(); it++)
					if (compare(k, it->first))
						return (it);
				return (it);
			}

			ft::pair<iterator, iterator>	equal_range(const key_type & k)
				{ return (ft::pair<iterator, iterator>(lower_bound(k), upper_bound(k))); }
			ft::pair<const_iterator, const_iterator>	equal_range(const key_type & k) const
				{ return (ft::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k))); }

			/*	Allocator	*/

			allocator_type	get_allocator() const { return (allocator_type()); }

		private :

			/*
			* The right rotation at node x makes x goes down in the right direction and as a result, its left child goes up.
			*/

			void	rightRotate(NodePtr x)
			{
				NodePtr	y = x->left;
				x->left = y->right;
				if (y->right != TNULL)
					y->right->parent = x;
				y->parent = x->parent;
				if (x->parent == nullptr)
					this->root = y;
				else if (x == x->parent->right)
					x->parent->right = y;
				else
					x->parent->left = y;
				y->right = x;
				x->parent = y;
			}

	
			void	leftRotate(NodePtr x)
			{
				NodePtr	y = x->right;
				x->right = y->left;
				if (y->left != TNULL)
					y->left->parent = x;
				y->parent = x->parent;
				if (x->parent == nullptr)
					this->root = y;
				else if (x == x->parent->left)
					x->parent->left = y;
				else
					x->parent->right = y;
				y->left = x;
				x->parent = y;
			}

			void	rbTransplant (NodePtr u, NodePtr v)
			{
				if (u->parent == nullptr)
					root = v;
				else if (u == u->parent->left)
					u->parent->left = v;
				else
					u->parent->right = v;
				v->parent = u->parent;
			}

			NodePtr	minimum(NodePtr node) const
			{
				if (!_size)
					return (TNULL);
				while (node->left != TNULL)
					node = node->left;
				return (node);
			}

			NodePtr	maximum(NodePtr node) const
			{
				if (!_size)
					return (TNULL);
				while (node->right != TNULL)
					node = node->right;
				return (node);
			}

			void	insertFix(NodePtr k)
			{
				NodePtr	u;
				while (k->parent->color == 1)
				{
					if (k->parent == k->parent->parent->right)
					{
						u = k->parent->parent->left;
						if (u->color == 1)
						{
							u->color = 0;
							k->parent->color = 0;
							k->parent->parent->color = 1;
							k = k->parent->parent;
						}
						else
						{
							if (k == k->parent->left)
							{
								k = k->parent;
								rightRotate(k);
							}
							k->parent->color = 0;
							k->parent->parent->color = 1;
							leftRotate(k->parent->parent);
						}
					}
					else
					{
						u = k->parent->parent->right;
						if (u->color == 1)
						{
							u->color = 0;
							k->parent->color = 0;
							k->parent->parent->color = 1;
							k = k->parent->parent;
						}
						else
						{
							if (k == k->parent->right)
							{
								k = k->parent;
								leftRotate(k);
							}
							k->parent->color = 0;
							k->parent->parent->color = 1;
							rightRotate(k->parent->parent);
						}
					}
					if (k == root)
						break;
				}
				root->color = 0;
			}

			void deleteFix(NodePtr x)
			{
				NodePtr	s;
				while (x != root && x->color == 0)
				{
					if (x == x->parent->left)
					{
						s = x->parent->right;
						if (s->color == 1)
						{
							s->color = 0;
							x->parent->color = 1;
							leftRotate(x->parent);
							s = x->parent->right;
						}
						if (s->left->color == 0 && s->right->color == 0)
						{
							s->color = 1;
							x = x->parent;
						}
						else
						{
							if (s->right->color == 0)
							{
								s->left->color = 0;
								s->color = 1;
								rightRotate(s);
								s = x->parent->right;
							}
							s->color = x->parent->color;
							x->parent->color = 0;
							s->right->color = 0;
							leftRotate(x->parent);
							x = root;
						}
					}
					else
					{
					
						s = x->parent->left;
						if (s->color == 1)
						{
							s->color = 0;
							x->parent->color = 1;
							rightRotate(x->parent);
							s = x->parent->left;
						}
						if (s->right->color == 0 && s->left->color == 0)
						{
							s->color = 1;
							x = x->parent;
						}
						else
						{
							if (s->left->color == 0)
							{
								s->right->color = 0;
								s->color = 1;
								leftRotate(s);
								s = x->parent->left;
							}
							s->color = x->parent->color;
							x->parent->color = 0;
							s->left->color = 0;
							rightRotate(x->parent);
							x = root;

						}
					}
				}
				x->color = 0;
			}

			NodePtr	searchTree(NodePtr node, const key_type & key) const
			{
				if (node == TNULL || key == node->data->first)
					return (node);
				if (compare(key, node->data->first))
					return (searchTree(node->left, key));
				return (searchTree(node->right, key));
			}
	};

	template< class Key, class T, class Compare, class Alloc >
	bool operator==( const ft::map<Key,T,Compare,Alloc>& lhs, const ft::map<Key,T,Compare,Alloc>& rhs) 	
		{ return ((lhs.size() == rhs.size()) && ft::equal(lhs.begin(), lhs.end(), rhs.begin())); }

	template< class Key, class T, class Compare, class Alloc >
	bool operator!=( const ft::map<Key,T,Compare,Alloc>& lhs, const ft::map<Key,T,Compare,Alloc>& rhs) 	
		{ return (!(lhs == rhs)); }

	template< class Key, class T, class Compare, class Alloc >
	bool operator<( const ft::map<Key,T,Compare,Alloc>& lhs, const ft::map<Key,T,Compare,Alloc>& rhs) 	
		{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); }

	template< class Key, class T, class Compare, class Alloc >
	bool operator<=( const ft::map<Key,T,Compare,Alloc>& lhs, const ft::map<Key,T,Compare,Alloc>& rhs) 	
		{ return (!(rhs < lhs)); }

	template< class Key, class T, class Compare, class Alloc >
	bool operator>( const ft::map<Key,T,Compare,Alloc>& lhs, const ft::map<Key,T,Compare,Alloc>& rhs) 	
		{ return (rhs < lhs); }

	template< class Key, class T, class Compare, class Alloc >
	bool operator>=( const ft::map<Key,T,Compare,Alloc>& lhs, const ft::map<Key,T,Compare,Alloc>& rhs) 	
		{ return (!(lhs < rhs)); }
};
