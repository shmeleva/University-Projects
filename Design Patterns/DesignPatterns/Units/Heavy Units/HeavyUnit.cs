using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    abstract class HeavyUnit : Unit
    {
        sealed public override String Type
        {
            get
            {
                return
                    "Тяжелый";
            }
        }

        protected HeavyUnit(int healthPoints = 100) : base(healthPoints) { }
    }
}
