
let S = [5,3,4,8,6,7]

func longestNonDecreasingSequence() {
    var state = [Int]()
    for (i,v) in S.enumerate() {
        if let m = (0..<i).filter({S[$0]<=v}).map({state[$0]}).maxElement() {
            state.append(m+1)
        } else {
            state.append(1)
        }
    }
    print(state[S.count-1])
    print(state)
}

longestNonDecreasingSequence()

struct LIS {

    var dp: [Int]
    var longest = 0
    let inf = 2000000000
    
    init(size: Int) {
        self.dp = Array(count: size, repeatedValue: inf)
    }
    
    mutating func add(x: Int) {
        var left = 1
        var right = longest
        while left <= right {
            let mid = left + ((right - left) >> 1)
            if dp[mid] <= x {
                left = mid + 1
            } else {
                right = mid - 1
            }
        }
        right += 1
        dp[right] = x
        longest = max(right,longest)
    }

}

var lis = LIS(size: S.count)
S.forEach { lis.add($0) }
print(lis.longest)
