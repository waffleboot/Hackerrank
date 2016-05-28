
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
