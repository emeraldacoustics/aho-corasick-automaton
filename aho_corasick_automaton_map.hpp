#include <map>
#include <queue>
#include <vector>

using namespace std;

template <class T>
class AhoCorasickAutomaton
{
public:
	struct Node
	{
	public:
		int len;
		int val;
		int slnk;
		int plnk;
		map<T, int> son;

		Node(void) : len(0), val(-1), slnk(0), plnk(0)
		{

		}

		int next(const T & c) const
		{
			auto itr = son.find(c);
			if (itr != son.end())
				return itr->second;
			else
				return 0;
		}
	};

	int sz;
	vector<Node> nodes;

	AhoCorasickAutomaton(void)
	{
		clear();
	}

	Node & operator [] (const int & x)
	{
		return nodes[x];
	}

	void clear(void)
	{
		sz = 0;
		nodes.clear();
		add_node();
	}

	int add_node(void)
	{
		nodes.push_back(Node());
		return sz++;
	}

	int insert(const T * first, const T * last, const int & id)
	{
		const T * & s = first;
		const int n = last - first;

		int u = 0;
		for(int i = 0; i < n; i++)
		{
			const T & c = s[i];
			int v = nodes[u].son[c];
			if (!v)
			{
				v = add_node();
				nodes[u].son[c] = v;
				nodes[v].len = nodes[u].len + 1;
			}
			u = v;
		}
		nodes[u].val = id;
		return u;
	}

	int next(int u, const T & c)
	{
		for (; u && !nodes[u].son.count(c); u = nodes[u].slnk);
		return nodes[u].next(c);
	}

	void build(void)
	{
		queue<int> que;
		for (const auto & p : nodes[0].son)
		{
			const int & u = p.second;
			que.push(u);
			nodes[u].slnk = 0;
			nodes[u].plnk = 0;
		}
		for (; !que.empty() ; que.pop())
		{
			const int & r = que.front();
			for (const auto & p : nodes[r].son)
			{
				const int & u = p.second;
				que.push(u);
				int v = nodes[r].slnk;
				v = nodes[u].slnk = next(v, p.first);
				nodes[u].plnk = nodes[v].val != -1 ? v : nodes[v].plnk;
			}
		}
	}

	vector<int> sorted_indices(void)
	{
		int mxlen = 0;
		for (int u = 0; u < sz; u++)
			mxlen = max(mxlen, nodes[u].len);
		vector<int> occ(mxlen + 1);
		for (int u = 0; u < sz; u++)
			occ[nodes[u].len]++;
		for (int i = 1; i <= mxlen; i++)
			occ[i] += occ[i - 1];
		vector<int> ans(sz);
		for (int u = 0; u < sz; u++)
			ans[--occ[nodes[u].len]] = u;
		return ans;
	}

	vector<int> sorted_indices(const vector<int> & qry)
	{
		int mxlen = 0;
		for (const auto & u : qry)
			mxlen = max(mxlen, nodes[u].len);
		vector<int> occ(mxlen + 1);
		for (const auto & u : qry)
			occ[nodes[u].len]++;
		for (int i = 1; i <= mxlen; i++)
			occ[i] += occ[i - 1];
		vector<int> ans(qry.size());
		for (const auto & u : qry)
			ans[--occ[nodes[u].len]] = u;
		return ans;
	}
};

