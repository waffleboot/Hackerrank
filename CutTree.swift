
struct DP {
    static let size = 50
    static let size2 = size * size
    var dp = Array(count: size2 * size, repeatedValue: -1)
    func index(u: Int, _ nson: Int, _ K: Int) -> Int {
        return u * DP.size2 + nson * DP.size + K
    }
    func get(u: Int, _ nson: Int, _ K: Int) -> Int {
        return dp[index(u, nson, K)]
    }
    mutating func put(u: Int, _ nson: Int, _ K: Int, _ count: Int) {
        dp[index(u, nson, K)] = count
    }
}

struct Graph {
    typealias Vector = [Int]
    var g = [Vector!]()
    var dp = DP()
    mutating func expand(index: Int) {
        while g.count <= index { g.append(nil) }
        if g[index] == nil { g[index] = Vector() }
    }
    mutating func add(u: Int, _ v: Int) {
        zeroedAdd(u-1, v-1)
    }
    mutating func zeroedAdd(u: Int, _ v: Int) {
        expand(u); g[u].append(v)
        expand(v); g[v].append(u)
    }
    mutating func dfs() {
        var q = Vector()
        var visited = Array(count: g.count, repeatedValue: 0)
        var tree: [Vector!] = Array(count: g.count, repeatedValue: nil)
        q.append(0)
        while !q.isEmpty {
            let u = q.removeLast()
            visited[u] = 1
            if let children = g[u] {
                for v in children {
                    guard visited[v] == 0 else { continue }
                    if tree[u] == nil { tree[u] = Vector() }
                    tree[u].append(v)
                    q.append(v)
                }
            }
        }
        (0..<g.count).forEach{ g[$0] = tree[$0] }
    }
    func children(u: Int) -> Vector {
        guard let children = g[u] else { return Vector() }
        return children
    }
    mutating func f(u: Int, _ nson: Int, _ K: Int) -> Int {
        guard K >= 0 else { return 0 }
        guard nson > 0 else { return K == 0 ? 1 : 0 }
        if case let ans = dp.get(u, nson, K) where ans != -1 {
            return ans
        }
        var ans = f(u, nson-1, K-1)
        let rson = g[u][nson-1]
        for J in 0...K {
            ans += f(rson, children(rson).count, J)*f(u, nson-1, K-J)
        }
        dp.put(u, nson, K, ans)
        return ans
    }
    mutating func g(u: Int, _ nson: Int, _ K: Int) -> Int {
        var ans = 0
        for son in 0..<nson {
            let v = g[u][son]
            ans += g(v, children(v).count, K)
            ans += f(v, children(v).count, K-1)
        }
        return ans
    }
    mutating func calc(K: Int) -> Int {
        return f(0, children(0).count, K)
             + g(0, children(0).count, K)
    }
    mutating func calcAtMost(K: Int) -> Int {
        var ans = 2
        for K in 1...K {
            ans += calc(K)
        }
        return ans
    }
}

var g = Graph()
g.zeroedAdd(0, 1)
g.zeroedAdd(0, 2)
g.zeroedAdd(1, 3)
g.zeroedAdd(1, 4)
g.zeroedAdd(1, 5)
g.zeroedAdd(0, 6)
g.dfs()
print(g.calcAtMost(4))

