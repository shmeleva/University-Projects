// Задание 1.
// Реализовать рекурсивную функцию для вычисления n-го члена последовательности,
// в которой каждый следующий член равен произведению двух предыдущих,
// а первые 2 члена равны соответственно 1 и 2.

let rec FirstTask x y n =
    if n = 1 then x 
    else if n = 2 then y 
    else FirstTask y (x * y) (n - 1) 

let n = 7 

let firstResult = FirstTask 1 2 n 

// Задание 2.
// Разработать программу, определяющую самую часто встречающуюся букву
// в заданном английском тексте. Считать, что строчные и прописные буквы одинаковы.

let alphabet =
    ['a'..'z']
    |> Set.ofList

let LetterFrequency (text : string) =
    text.ToLower().ToCharArray()
    |> Array.filter(fun x -> alphabet.Contains(x))
    |> Seq.countBy(fun x -> x)
    |> Seq.sortBy(fun (x, y) -> -y)

    // Для лабы.

let text = "The sky is an immortal tent built by the Sons of Los"
let (letter, frequency) = Seq.head (LetterFrequency text)

// Задание 3.
// Дано действительное b > 0. Последовательность a1, a2, ... образована по следующему закону:
// a1 = b, ai = ai-1 - 1/sqr(i), i = 2, 3, ... .
// Найти первые k отрицательных членов последовательности.

let rec GetBorder b i (sum : float) =
    let currentSum = (sum + 1.0 / sqrt(float i)) 
    if currentSum > float b then (i, sum)
    else GetBorder b (i + 1) currentSum

let rec CountNegatives k b (i, sum : float) =
    if k = 0 then []
    else List.append [float b - float sum - 1.0/(sqrt(float i))] (CountNegatives (k - 1) b ((i + 1) , (sum + 1.0/(sqrt(float i)))))

let k = 5
let b = 2

let elements = CountNegatives k b (GetBorder b 2 0.0)

// Задание 4.
// Реализовать функцию, которая суммирует все натуральные числа меньше
// 1000, которые кратны 3 и 5.

let max = 1000
let sum = (List.sum (List.filter (fun x -> x % 3 = 0 || x % 5 = 0) [1..(max - 1)]))

// Задание 5.
// Реализовать функцию нахождения минимального числа, которое делится на все числа от 1 до 20.

let divisors = [(int64 1)..(int64 20)]

let rec Gcd (a : int64) (b : int64) =
    if b = (int64 0) then a
    else Gcd b (a % b)

let Lcm (a : int64) (b : int64) =
    abs(a * b) / (Gcd a b)

let result = List.fold (fun a b -> Lcm a b) (int64 1) divisors

// Задание 6.
// Реализовать функцию, которая определяет количество решения уравнения a*x^2 + b*x = c.

let Discriminant a b c =
    (b * b) - (4 * a * c)

let Solving a b c =
    let d = Discriminant a b -c 
    if d < 0 then 0
    else if d > 0 then 2
    else 1   

let Inflexion number =
    if number = 0 then "й"
    else if number = 1 then "е"
    else "я"

let _a = 2
let _b = 3
let _c = -2

let solving = Solving _a _b _c

// Задание 7.
// Реализовать функцию, которая находит сумму всех таких чисел (меньше 1
// миллиона) которые являются палиндромами в десятеричной и двоичной
// системе счисления.

let rec ToBinary x =
    if x = 0 then []
    else if x % 2 = 0 then List.append (ToBinary (x / 2)) [0]
    else List.append (ToBinary (x / 2)) [1]

let ReverseString (s: string) =
    System.String(Array.rev (s.ToCharArray()))

let IfPalindromeNumber x =
    let s = x.ToString()
    s = (ReverseString s)

let IfPalindromeList x =
    (List.rev x) = x

let _sum = (List.sum (List.filter (fun x -> IfPalindromeNumber x && IfPalindromeList (ToBinary x)) [1..999999]))

[<EntryPoint>]
let main argv =
    // 1.
    printfn "%d-й элемент последовательности: %d.\n" n firstResult

    // 2.
    printfn "Символ \"%c\" встречаеся %d раз.\n" letter frequency

    // 3.
    printfn "Первые %d отрицательных членов последовательности:\n%A.\n" k elements

    // 4.
    printfn "Сумма натуральных чисел меньше %d: %d.\n" max sum

    // 5.
    printfn "Минимальное число, которое делится на все числа от 1 до %d: %d.\n" 20 result

    // 6.
    printfn "Уравнение %d * x^2 + %d * x = %d имеет %d решени%s.\n" _a _b _c solving (Inflexion solving)

    // 7.
    printf "Сумма чисел до миллиона, которые являются палиндромами в десятеричной и двоичной системах счисления равна %d.\n" _sum

    0


