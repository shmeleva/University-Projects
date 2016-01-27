using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    public class ChangeCannotBeReturnedException : Exception
    {
        public ChangeCannotBeReturnedException() : base("Change can't be returned") { }
    }
}
