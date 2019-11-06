procedure ConformantArray is
   type ArrType is array (Natural range <>) of Integer;
   type ArrPtr is access ArrType;
   A : ArrPtr;
   procedure Inner(Arr: in ArrPtr) is
   begin
      for I in 1..Arr'Last loop
         Arr(I) := 7; -- 7 is just easy to find in the assembly
      end loop;
   end; -- end Inner
begin
   A := new ArrType(1..10);
   Inner(A);
end; --
