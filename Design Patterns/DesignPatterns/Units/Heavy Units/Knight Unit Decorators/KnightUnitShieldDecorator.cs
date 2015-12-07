using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Щит.
    /// </summary>
    class KnightUnitShieldDecorator : KnightUnitBaseDecorator
    {
        public KnightUnitShieldDecorator(KnightUnit knight) : base(knight) { }

        public override string Name
        {
            get
            {
                return base.Name + " с щитом";
            }
        }

        public override Int32 Defence
        {
            get
            {
                return base.Defence + 2;
            }
        }

        public override Unit CloneModifiedUnit(Int32 healthPoints)
        {
            return new KnightUnitShieldDecorator((KnightUnit)_knight.CloneModifiedUnit(healthPoints));
        }
    }
}
