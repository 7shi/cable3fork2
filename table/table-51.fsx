let bios = System.IO.File.ReadAllBytes "../../../bios"
let baseaddr = 0xf0000
let start = baseaddr + 0x100
let last = start + bios.Length
let read32 addr = System.BitConverter.ToInt32(bios, addr - start)
let slice addr size = bios.[addr - start .. addr + size - 1 - start]

let tables =
   Seq.initInfinite (fun i -> i, read32 (start + 3 + 4 * i))
|> Seq.takeWhile (fun (_, addr) -> baseaddr <= addr && addr <= last)
|> Seq.sortBy (fun (_, addr) -> addr)
|> fun s -> Seq.append s (seq {yield Seq.length s, last})
|> Seq.pairwise
|> Seq.map (fun ((i0, ad0), (_, ad1)) ->
    i0, ad0, if ad0 < start then 0 else ad1 - ad0)
|> Seq.sortBy (fun (index, _, _) -> index)
|> Seq.toArray

let data = tables |> Array.map (fun (_, addr, size) -> slice addr size)
let data8  = data.[ 8] |> Array.mapi (fun i b -> i, int b)
let data51 = data.[51] |> Array.mapi (fun i b -> i, int b)

for i in 0 .. int <| Array.max data.[8] do
    let optypemap =
       data8
    |> Seq.filter (fun (_, b) -> b = i)
    |> Seq.map fst
    |> Seq.toArray
    data51
    |> Seq.filter (fun (_, b) -> Array.exists ((=) b) optypemap)
    |> Seq.map (fun (i, _) -> sprintf "%02x" i)
    |> String.concat ","
    |> printfn "%d\t%s" i
