func bitsCount(num: Int) -> Int {
    var x = num, count = 0
    while x > 0 {
        x -= x & -x
        count += 1
    }
    return count
}

func bitsFactorization(num: Int) throws -> [Int] {
    var x = num
    var result = [Int]()
    while x > 0 {
        let bit = x & -x
        switch bit {
        case 1: result.append(0)
        case 2: result.append(1)
        case 4: result.append(2)
        case 8: result.append(3)
        case 16: result.append(4)
        case 32: result.append(5)
        case 64: result.append(6)
        case 128: result.append(7)
        case 256: result.append(8)
        case 512: result.append(9)
        case 1024: result.append(10)
        case 2048: result.append(11)
        case 4096: result.append(12)
        case 8192: result.append(13)
        case 16384: result.append(14)
        case 32768: result.append(15)
        case 65536: result.append(16)
        default:
            enum BitFactorizationError: ErrorType { case UnsupportedBit(Int) }
            throw BitFactorizationError.UnsupportedBit(bit)
        }
        x -= bit
    }
    return result
}

func elementsOnPositions(elements: Int, _ positions: Int) throws -> [[Int]] {
    if positions == 0 {
        return [[Int]]()
    } else if positions == elements {
        return Array(arrayLiteral: Array(0..<elements))
    } else {
        var result = [[Int]]()
        for num in 1..<(1<<elements) {
            if bitsCount(num) == positions {
                try result.append(bitsFactorization(num))
            }
        }
        return result
    }
}

func split(num: Int, position: Int, add: (index:Int,value:Int)->Void, done: ()->Void) {
    if position == 1 {
        add(index:position-1, value:num)
        done()
    } else {
        for i in 0...num {
            add(index: position-1, value: i)
            split(num-i, position: position-1, add: add, done: done)
        }
    }
}

func split(num: Int, positions: Int) -> [[Int]] {
    var result = [[Int]]()
    var data = Array(count: positions, repeatedValue: 0)
    split(num, position: positions, add: { data[$0] = $1 }) { result.append(data) }
    return result
}

func p(depth: Int, _ string: String) {
    let s = String(count:depth*2, repeatedValue: Character(" "))
    print("\(s)\(string)")
}

struct DP {
    var dp = [[Int]]()
    init() {
        (1...55).forEach{ _ in dp.append(Array(count: 55, repeatedValue: -1)) }
    }
    mutating func get(x: Int, _ y: Int) -> Int {
        return dp[x][y]
    }
    mutating func put(x: Int, _ y: Int, _ v: Int) {
        dp[x][y] = v
    }
}

struct Graph: CustomStringConvertible {
    typealias Vector = [Int]
    var g = [Vector?]()
    var dp1 = DP()
    var dp2 = DP()
    mutating func expand(a: Int) {
        while g.count <= a {
            g.append(nil)
        }
        guard g[a] == nil else { return }
        g[a] = Vector()
    }
    mutating func add(a: Int, _ b: Int) {
        expand(a); g[a]?.append(b)
        expand(b); g[b]?.append(a)
    }
    mutating func dfs() {
        var queue = [Int]()
        var visited = Array(count: g.count, repeatedValue: 0)
        var tree : [Vector!] = Array(count: g.count, repeatedValue: nil)
        queue.append(0)
        while !queue.isEmpty {
            let u = queue.removeLast()
            visited[u] = 1
            if let neighbours = g[u] {
                for v in neighbours {
                    if visited[v] == 0 {
                        if tree[u] == nil {
                            tree[u] = Vector()
                        }
                        tree[u].append(v)
                        queue.append(v)
                    }
                }
            }
        }
        (0..<g.count).forEach{ g[$0] = tree[$0] }
    }
    mutating func calc(K: Int) throws -> Int {
        var ans = 0
        for k in 1...K {
            ans += try f(0, k)
            ans += try f2(0, k)
        }
        return ans
    }
    mutating func f2(u: Int, _ K: Int) throws -> Int {
        var ans = dp2.get(u, K)
        if ans == -1 {
            ans = 0
            if let children = g[u] {
                if K > 1 {
                    try children.forEach{ ans += try f($0, K - 1) }
                }
                try children.forEach{ ans += try f2($0, K) }
            }
            if K == 1 && u != 0 {
                ans += 1
            }
            dp2.put(u, K, ans)
        }
        return ans
    }
    mutating func f(u: Int, _ K: Int, _ depth: Int = 0) throws -> Int {
        var ans = dp1.get(u, K)
        if ans == -1 {
            ans = 0
            //p(depth,"f(\(u),\(K))")
            if let children = g[u] {
                ans += try ff(children,K,depth)
            }
            dp1.put(u, K, ans)
        }
        return ans
    }
    func factorial(n: Int) -> Int {
        return n == 0 ? 1 : n * factorial(n-1)
    }
    func cnk(n: Int, _ k: Int) -> Int {
        return factorial(n)/(factorial(k)*factorial(n-k))
    }
    mutating func ff(children: Vector, _ K: Int, _ depth: Int) throws -> Int {
        var ans = 0
        for L in 0...children.count {
            let nextK = K-(children.count-L)
            if nextK == 0 {
                let d = cnk(children.count, L)
                //p(depth, "L=\(L) cnk(\(children.count),\(L))=\(d)")
                ans += d
            } else if nextK > 0 {
                let variants = try elementsOnPositions(children.count, L)
                //p(depth,"L=\(L) variants=\(variants)")
                for variant in variants {
                    let choose = variant.map{ children[$0] }
                    let splitResult = split(nextK, positions: L)
                    //p(depth, "L=\(L) \(choose) cut=\(children.count-L) nextK=\(nextK) split=\(splitResult)")
                    for splitItem in splitResult {
                        var ret = 1
                        var cnt = 0
                        for i in 0..<L {
                            if splitItem[i] != 0 {
                                ret *= try f(choose[i], splitItem[i], depth+1)
                                cnt += 1
                            }
                        }
                        if cnt > 0 {
                            ans += ret
                        }
                    }
                }
            }
        }
        //print("ans=\(ans)")
        return ans
    }
    var description: String {
        return String(g)
    }
}

func read() -> [Int] {
    return readLine()!.characters.split(" ").map(String.init).map{Int($0)!}
}

var g = Graph()
let nk = read()
let n = nk[0], k = nk[1]
for _ in 1..<n {
    let ab = read()
    let a = ab[0], b = ab[1]
    g.add(a-1,b-1)
}
g.dfs()
