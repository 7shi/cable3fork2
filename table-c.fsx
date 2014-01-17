let bios = System.IO.File.ReadAllBytes "../../bios"
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

printfn "#include <stdint.h>"
printfn ""
for index, addr, size in tables do
    printf "uint8_t table%02d[] = { /* %x */" index addr
    for i in 0 .. size - 1 do
        if i % 16 = 0 then
            printfn ""
            printf "   "
        printf " 0x%02x," bios.[addr + i - start]
    printfn " };"
printfn ""
printf "uint8_t *tables[] = {"
for index, _, _ in tables do
    if index % 10 = 0 then
        printfn ""
        printf "   "
    printf " table%02d," index
printfn " };"
