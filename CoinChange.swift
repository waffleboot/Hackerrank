
let V = [1,2,4,6]
let S = 37

func dp1() {
    var state = Array(arrayLiteral: 0)
    for i in 1...S {
        if let m = V.filter({$0<=i}).map({state[i-$0]}).filter({$0>=0}).minElement() {
            state.append(m+1)
        } else {
            state.append(-1)
        }
    }
    print(state[S])
    print(state)
}

dp1()

func dp2() {
    var state = Array(count: S+1, repeatedValue: -1)
    state[0] = 0
    for v in V {
        for i in 0...S where v <= i {
            if state[i-v] == -1 {
                state[i] == -1
            } else if state[i] == -1 {
                state[i] = state[i-v] + 1
            } else {
                state[i] = min(state[i-v] + 1,state[i])
            }
        }
    }
    print(state[S])
    print(state)
}

dp2()
