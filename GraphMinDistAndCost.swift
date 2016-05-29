
// graph has nodes, edges, weights on edges and costs on nodes
// find min distance path for available money
// for equal distances return min cost path

struct Edge: CustomStringConvertible {
    let u: Int
    let v: Int
    let weight: Int
    var description: String {
        return "\(u+1) -> \(v+1) = \(weight)"
    }
}

struct Cell<T: Equatable>: Equatable, CustomStringConvertible {
    let row: Int
    let column: Int
    let value: T
    var description: String {
        return "[\(row+1),\(column)]=\(value)"
    }
}

func ==<T:Equatable>(lhs: Cell<T>, rhs: Cell<T>) -> Bool {
    return lhs.row == rhs.row && lhs.column == rhs.column && lhs.value == rhs.value
}

struct Matrix<T: Comparable>: CustomStringConvertible {
    
    var data: [T]
    let rowsCount: Int
    let columnsCount: Int
    
    init(rowsCount: Int, columnsCount: Int, initial: T) {
        self.rowsCount = rowsCount
        self.columnsCount = columnsCount
        self.data = Array(count: rowsCount * columnsCount, repeatedValue: initial)
    }
    
    subscript(row: Int, column: Int) -> T {
        get {
            return data[row * columnsCount + column]
        }
        set {
            data[row * columnsCount + column] = newValue
        }
    }
    
    func filter(f: (row: Int, column: Int) -> Bool) -> [Cell<T>] {
        var ret = [Cell<T>]()
        for row in 0..<rowsCount {
            let start = row * columnsCount
            for col in 0..<columnsCount {
                if f(row: row, column: col) {
                    ret.append(Cell(row: row, column: col, value: data[start+col]))
                }
            }
        }
        return ret
    }
    
    var description: String {
        var s = ""
        for row in 0..<rowsCount {
            s += "\n\(self[row,0])"
            for col in 1..<columnsCount {
                s += ","
                s += String(self[row,col])
            }
        }
        return s
    }
    
}

struct Graph {

    var prices = [Int]()
    var nodes = [[Edge]!]()
    
    mutating func addPrice(u: Int, price: Int) {
        while u > prices.count { prices.append(0) }
        prices[u-1] = price
    }
    
    mutating func add(u: Int, _ v: Int, weight: Int) {
        let m = max(u,v)
        while m > nodes.count { nodes.append(nil) }
        if var unodes = nodes[u-1] {
            unodes.append(Edge(u: u, v: v-1, weight: weight))
            nodes[u-1] = unodes
        } else {
            nodes[u-1] = Array(arrayLiteral: Edge(u: u, v: v-1, weight: weight))
        }
        if var vnodes = nodes[v-1] {
            vnodes.append(Edge(u: v, v: u-1, weight: weight))
            nodes[v-1] = vnodes
        } else {
            nodes[v-1] = Array(arrayLiteral: Edge(u: v, v: u-1, weight: weight))
        }
    }
    
    func getNodesFor(u: Int) -> [Edge] {
        return nodes[u]
    }
    
    func shortPath(M: Int) -> (path:Int,cost:Int) {
        var checked = [Cell<Int>]()
        var visited = Matrix(rowsCount: nodes.count, columnsCount: M+1, initial: 0)
        var minPath = Matrix(rowsCount: nodes.count, columnsCount: M+1, initial: Int.max)
        minPath[0,M] = 0
        checked.append(Cell(row: 0, column: M, value: 0))
        while true {

            var j = -1
            var minIndex = Cell(row: -1, column: -1, value: Int.max)
            for i in 0..<checked.count {
                let c = checked[i]
                if visited[c.row,c.column] == 0 {
                    let m = minPath[c.row,c.column]
                    if m < minIndex.value {
                        minIndex = Cell(row: c.row, column: c.column, value: m)
                        j = i
                    }
                }
            }
            if j < 0 {
                break
            }

            visited[minIndex.row,minIndex.column] = 1
            checked.removeAtIndex(j)

            print(minIndex)

            for p in getNodesFor(minIndex.row) {
                let remain = minIndex.column - prices[p.v]
                let newdist = minIndex.value + p.weight
                if remain >= 0 && newdist < minPath[p.v,remain] {
                    minPath[p.v,remain] = newdist
                    checked.append(Cell(row: p.v, column: remain, value: 0))
                    for d in 0..<remain where minPath[p.v,d] >= newdist {
                        //print("clean")
                        visited[p.v,d] = 1
                    }
                }
            }
        }
        print("done")
        var path = Int.max
        var cost = Int.max
        for i in 0...M {
            let c = minPath[nodes.count-1,i]
            if c < path {
                path = c
                cost = i
            } else if c == path {
                cost = max(cost,i)
            }
        }
        return (path:path,cost:M-cost)
    }
    
}

var g = Graph()

g.add(1, 2, weight: 1)
g.add(1, 3, weight: 2)
g.add(2, 4, weight: 1)
g.add(3, 4, weight: 1)

g.addPrice(1, price: 0)
g.addPrice(2, price: 9)
g.addPrice(3, price: 2)
g.addPrice(4, price: 0)

print(g.shortPath(30))


