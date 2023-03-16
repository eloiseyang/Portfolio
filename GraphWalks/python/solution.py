"""
02/15/2023
Graph Walks Solution
EFJY
"""
import csv


class Graph:
    """
    A class for directed acyclic graphs with edge labels and 
    dictionary-like indexing for vertices by id, and for edge labels by
    head-tail pairs and vice versa. It can be initialized by a list of
    edges formatted (head, tail, label) or by a CSV file with the same
    format.
    """
    class _Vertex:
        """Private class storing vertex id, parents, and children."""
        def __str__(self):
            return "(parents: {}, children: {})".format(self.parents,
                                                        self.children)
        def __init__(self, i):
            self.i = i
            self.parents = []
            self.children = []


    def __init__(self, edgeIn):
        """Initialize by lists of (head, tail, label)."""
        edgeMap = {}
        vertexMap = {}
        roots = set()
        vertices = set()
        edges = set()

        for e in edgeIn:
            head = e[0]
            tail = e[1]
            label = e[2]
            edge = (head,tail, )
            headKey = self._keyify(head)
            tailKey = self._keyify(tail)
            labelKey = self._keyify(label)
            edgeKey = self._keyify(edge)
            if tailKey not in vertexMap:
                vertexMap[tailKey] = self._Vertex(tail)
                vertices.add(tail)
            if headKey not in vertexMap:
                vertexMap[headKey] = self._Vertex(head)
                vertices.add(head)
            if labelKey not in edgeMap:
                edgeMap[labelKey] = []
            edges.add(edge)
            vertexMap[headKey].children.append(tail)
            vertexMap[tailKey].parents.append(head)
            edgeMap[edgeKey] = label
            edgeMap[labelKey].append(edge)

        for v in vertices:
            key = self._keyify(v)
            if len(vertexMap[key].parents) == 0:
                roots.add(v)

        self.vertices = vertices
        self.edges = edges
        self._roots = roots
        self._map = edgeMap|vertexMap
        self._stack = list(self._roots)
        self._visited = [False]*len(self.vertices)

    @classmethod
    def fromCSV(cls, filename):
        """Initialize by csv file."""
        with open(filename, newline='') as csvfile:
            reader = csv.reader(csvfile)
            edges = []
            next(reader)
            for row in reader:
                edges.append([int(row[0]), int(row[1]), row[2]])
        return Graph(edges)

    def _keyify(self, k):
        """Creates frozenset keys for dictionary indexing."""
        if type(k) is frozenset:
            return k
        elif type(k) is tuple:
            return frozenset(k)
        elif type(k) is int or str:
            return frozenset((k, ))
        else:
            raise Exception("A Graph key must be a tuple,"
                + "an integer, or a string.")

    def keys(self):
        """Prints Graph keys as a list."""
        r = []
        for m in self._map:
            mi = iter(m)
            if len(m) == 1:
                r.append(next(mi))
            elif len(m) == 2:
                r.append((next(mi), next(mi), ))
        return r

    def __iter__(self):
        return self

    def __next__(self):
        """Iterator that returns vertices in reverse topological order."""
        while len(self._stack) > 0:
            v = self._stack.pop()
            v = self[v]
            if not all([self._visited[c] for c in v.children]):
                self._stack.append(v.i)
                for c in v.children:
                    if self._visited[c] == False:
                        self._visited[c] = True
                        self._stack.append(c)
            else:
                return v
        self._stack = list(self._roots)
        self._visited = [False]*len(self.vertices)
        raise StopIteration

    def __getitem__(self, key):
        return self._map[self._keyify(key)]

    def __str__(self):
        s = [str(k) + ": " + str(self[k]) for k in self.keys()]
        return '('+ ', '.join(s) + ')'


def totalWalks(graph, fr='B', to='C'):
    """Solution to given problem. Counts every walk on a given graph
    connecting 'B' to 'C'. By accumulating the number of 'C' edges a
    given vertex is directed towards in reverse topological order,
    this function finds every relevant walk in O(V + E)."""
    count = {}
    for v in graph:
        count[v.i] = 0
    for v in graph:
        for c in v.children:
            if graph[c,v.i] == to:
                count[v.i] += 1
        for p in v.parents:
            count[p] += count[v.i]

    total = 0
    for e in graph[fr]:
        total += count[e[1]]
    return total


if __name__ == "__main__":
    f = 'graph_coo.csv'
    g = Graph.fromCSV(f)
    print(totalWalks(g))
