open System;
open System.IO

//  Чтение и записи

let ReadFile filename =
    File.ReadAllText (filename)

let WriteFile (filename: string, str: string) =
    File.WriteAllText (filename, str)

// Построение дерева кодов и преобразование дерева в таблицу

type Tree =
    | Leaf of int * char
    | Node of int * Tree * Tree
 
let Frequency = function
    | Leaf (f, _) -> f
    | Node (f, _, _) -> f

let FrequencyCompare a b = compare (Frequency a) (Frequency b)
 
let BuildTree letterFrequency =
    let leaves = List.map (fun (c,f) -> Leaf (f,c)) letterFrequency

    let FrequencySort = List.sortWith FrequencyCompare

    let rec Build = function
        | [] -> failwith "Empty list."
        | [a] -> a
        | a::b::tail ->
            let node = Node(Frequency a + Frequency b, a, b)
            Build (FrequencySort(node::tail))

    let sortedLeaves = FrequencySort leaves
    Build (sortedLeaves)
 
let rec PrintTree = function
  | code, Leaf (f, c) -> printfn "%c\t%d\t%s" c f (String.concat "" (List.rev code))
  | code, Node (_, l, r) ->
      PrintTree ("0"::code, l)
      PrintTree ("1"::code, r)

let rec CreateTable = function
  | code, Leaf (f, c) ->  [(c, String.concat "" (List.rev code))]
  | code, Node (_, l, r) -> List.concat [CreateTable ("0"::code, l); CreateTable ("1"::code, r)]
 
let LetterFrequency str =
  str 
  |> Seq.countBy (fun letter -> letter)
  |> Map.ofSeq

// Преобразования битовых последовательностей

let rec To7Bits (str : string) =
    if str.Length % 7 = 0 then str 
    else To7Bits (str + "0")

let rec Restore7Bits (str : string) =
    if str.Length % 7 = 0 then str 
    else Restore7Bits ("0" + str)

let rec BinaryToDecimal (binary : string) decimal =
    if binary.Length = 0 then decimal
    else
        match binary.Chars 0 with
            | '0' -> BinaryToDecimal (binary.Substring 1) (decimal * 2)
            | '1' -> BinaryToDecimal (binary.Substring 1) (decimal * 2 + 1)
            | _   -> 0   

let rec DecimalToBinary decimal =
    match decimal with
    | 0 | 1 -> string decimal
    | _ ->
        let bit = string (decimal % 2)
        (DecimalToBinary (decimal / 2)) + bit

let rec EncodedToBytes (str : string) (numberOfOctets : int) octetCounter =
    if octetCounter = numberOfOctets then []
    else List.append [byte (BinaryToDecimal str.[0..6] 0)] (EncodedToBytes str.[7..str.Length - 1] numberOfOctets (octetCounter + 1))

// Кодирование файла

let EncodeMain inputFile outputFile =
    
    let input = File.ReadAllText (inputFile)

    let tree = LetterFrequency input |> Map.toList |> BuildTree

    let table = CreateTable ([], tree)
    let tableToString =
        List.fold (fun result (letter : char, code : string) ->  result + letter.ToString() + code + Environment.NewLine) "" table
    
    let EncodeLetter letterToEncode =
        (List.find (fun (letter, code) -> letter = letterToEncode) table) |> snd

    let encoded = List.ofSeq input |> List.map EncodeLetter |> List.fold (+) ""
    let encodedTo7Bits = To7Bits encoded
    let bytes = EncodedToBytes encodedTo7Bits (encodedTo7Bits.Length /  7) 0
    
    let encodedToString =
        table.Length.ToString() +
        Environment.NewLine +
        tableToString +
        encoded.Length.ToString() +
        Environment.NewLine +
        (System.Text.Encoding.ASCII.GetString (List.toArray bytes)) 

    File.WriteAllText (outputFile, encodedToString)

// Декодирование файла

let DecodeMain (inputFile : string) outputFile =
    
    use sr = new StreamReader(inputFile)

    let firstString = sr.ReadLine ()
    let tableSize = System.Int32.Parse <| firstString

    let ReadTable = seq {  
        for i in 1 .. tableSize do 
            yield sr.ReadLine () 
    }

    let decodingTable = Seq.toArray <| Seq.map (fun (s : string) -> (s.[0],s.[1..])) ReadTable

    let textLenght = System.Int32.Parse <| sr.ReadLine()

    let ReadText = seq {
        while not sr.EndOfStream do
            yield sr.ReadLine ()
    }

    let encoded = (String.concat "" (Seq.toArray ReadText)).ToCharArray()
    let bytes = System.Text.Encoding.ASCII.GetBytes encoded
    let bytesToBinary = Array.map (fun c -> Restore7Bits (DecimalToBinary c)) <| Array.map (fun c -> (int c)) encoded
    let bytesToBinaryConcat = String.concat "" bytesToBinary
    let stringToDecode = bytesToBinaryConcat.[0..textLenght-1]

    let GetCode codeToDecode =
        (Array.find (fun (letter, code) -> code = codeToDecode) decodingTable) |> fst 

    let mutable decoded = "" 

    let rec DecodeString table (stringToDecode: string) =
        for line in table do
            if stringToDecode.Length <> 0 && stringToDecode.StartsWith(snd line) then
                decoded <- decoded + string (fst line)
                (DecodeString table stringToDecode.[(snd line).Length..stringToDecode.Length - 1])

    DecodeString decodingTable stringToDecode
    File.WriteAllText (outputFile, decoded)

[<EntryPoint>]
let main argv = 
    if argv.Length <> 3
    then
        printf "%s" "Error!"
        -1
    else
        let inputFile = string (argv.GetValue(0))
        let outputFile = string (argv.GetValue(1))
        let mode = string (argv.GetValue(2))

        if mode = "-e" then
            EncodeMain inputFile outputFile
        else if mode = "-d" then
            DecodeMain inputFile outputFile
        else
            printf "%s" "Error!"
        
        0
