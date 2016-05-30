
let Coins = [1,2,4,6]
let Sum = 37

func dp1() {
    var state = Array(arrayLiteral: 0) // state[0] = 0
    for currentSum in 1...Sum {
        //                                                                               state could be -1, look at row 12
        if let prev = Coins.filter({$0<=currentSum}).map({state[currentSum-$0]}).filter({$0>=0}).minElement() {
            state.append(prev+1)
        } else {
            state.append(-1)
        }
    }
    print(state[Sum])
    print(state)
}

dp1()

func dp2() {
    var state = Array(count: Sum+1, repeatedValue: -1)
    state[0] = 0
    for coin in Coins {
        for currentSum in 0...Sum where coin <= currentSum {
            let prevCount = state[currentSum-coin]
            if prevCount == -1 {
                state[currentSum] == -1
            } else if state[currentSum] == -1 {
                state[currentSum] = prevCount + 1
            } else {
                state[currentSum] = min(prevCount+1,state[currentSum])
            }
        }
    }
    print(state[Sum])
    print(state)
}

dp2()
