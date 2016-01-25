using System;

namespace VendingMachine.Exceptions
{
    public class VendingMachineException : Exception
    {
        public VendingMachineException(string message) : base(message) { }
    }
}
