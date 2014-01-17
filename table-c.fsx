let bios = System.IO.File.ReadAllBytes "../../bios"
let start = 0xf0100
let read8 addr = bios.[addr - start]
let read32 addr = System.BitConverter.ToInt32(bios, addr - start)
let tables = [for i in 0..51 -> i, read32 (start + 3 + 4 * i)]
let byaddr = List.sortBy (fun (_, b) -> b) tables
let byaddr2 = List.concat [byaddr.Tail; [-1, start + bios.Length]]
let getsize (a, b) (_, d) = b, a, if b < start then 0 else d - b
let sizes = List.map2 getsize byaddr byaddr2
let byindex = List.sortBy (fun (_, b, _) -> b) sizes
let maxsize = List.max (List.map (fun (_, _, c) -> c) byindex)
printfn "unsigned char tables[%d][%d] = {" byindex.Length maxsize
for addr, index, size in byindex do
    printf "    {   /* [%d] %x */" index addr
    for i in 0..size - 1 do
        if i % 16 = 0 then
            printfn ""
            printf "       "
        printf " 0x%02x," (read8 (addr + i))
    printfn ""
    printfn "    },"
printfn "};"
