let bios = System.IO.File.ReadAllBytes "../../bios"
for i in 0..99 do
    printfn "%d\t%x" i (System.BitConverter.ToInt32(bios, 3 + i * 4))
