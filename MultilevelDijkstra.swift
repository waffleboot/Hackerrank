func read() -> [Int] {
    return readLine()!.characters.split(" ").map(String.init).map{Int($0)!}
}
let nmk = read()
let (n,m,k) = (nmk[0],nmk[1],nmk[2])
var fishes = Array(count: n, repeatedValue: 0)
for i in 0..<n {
    var mask = 0
    let f = read()
    for j in 1..<f.count {
        mask |= 1<<(f[j]-1)
    }
    fishes[i] = mask
}
struct Edge {
    let v: Int
    let z: Int
    init(_ v: Int, _ z: Int) {
        self.v = v
        self.z = z
    }
}
struct Graph {
    var edges = [[Edge]!]()
    mutating func add(u: Int, _ edge: Edge) {
        while edges.count <= u {
            edges.append(nil)
        }
        if var es = edges[u] {
            es.append(edge)
            edges[u] = es
        } else {
            edges[u] = Array(arrayLiteral: edge)
        }
    }
    mutating func add(a: Int, _ b: Int, _ z: Int) {
        add(a,Edge(b,z))
        add(b,Edge(a,z))
    }
    func allEdges(u: Int) -> [Edge] {
        return edges[u]
    }
}
var g = Graph()
for _ in 0..<m {
    let d = read()
    let (a,b,z) = (d[0]-1,d[1]-1,d[2])
    g.add(a,b,z)
    g.add(b,a,z)
}
let t = (1<<k)
struct Dist {
    var data = Array(count: n*t, repeatedValue: Int.max)
    subscript(city: Int, mask: Int) -> Int {
        get {
            return data[t*city + mask]
        }
        set {
            data[t*city+mask] = newValue
        }
    }
}
var dist = Dist()
struct Label {
    let index: Int
    var city: Int {
        return index / t
    }
    var mask: Int {
        return index % t
    }
}
struct Labels {
    var q = [Label]()
    mutating func add(label: Label) {
        q.append(label)
    }
    mutating func min() -> Label {
        var remove = 0
        var minDist = Int.max
        var ret: Label!
        for (j,x) in q.enumerate() {
            let value = dist[x.city,x.mask]
            if value < minDist {
                minDist = value
                remove = j
                ret = x
            }
        }
        q.removeAtIndex(remove)
        return ret
    }
}
let p = fishes[0]
dist[0,p] = 0
var labels = Labels()
labels.add(Label(index:p))
while !labels.q.isEmpty {
    let label = labels.min()
    let oldDist = dist[label.city,label.mask]
    for edge in g.allEdges(label.city) {
        let newDist = oldDist + edge.z
        let newMask = label.mask | fishes[edge.v]
        if newDist < dist[edge.v,newMask] {
            dist[edge.v,newMask] = newDist
            labels.add(Label(index: t*edge.v+newMask))
        }
    }
}
var ans = Int.max
for i in 0..<t {
    for j in 0..<t {
        if (i | j) == t-1 {
            let a = dist[n-1,i]
            let b = dist[n-1,j]
            ans = min(ans,max(a,b))
        }
    }
}
print(ans)
