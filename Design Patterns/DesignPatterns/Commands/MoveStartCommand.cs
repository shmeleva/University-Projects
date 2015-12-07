using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class MoveStartCommand : Command
    {
        public MoveStartCommand() : base() { }

        protected override void _Do() { }

        protected override void _Undo() { }

        public override string ToString()
        {
            return "Начало нового этапа боя.";
        }
    }
}
