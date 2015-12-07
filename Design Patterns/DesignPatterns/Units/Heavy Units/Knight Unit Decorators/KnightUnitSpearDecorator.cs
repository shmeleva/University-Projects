using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Копье.
    /// </summary>
    class KnightUnitSpearDecorator : KnightUnitBaseDecorator
    {
        public KnightUnitSpearDecorator(KnightUnit knight) : base(knight) { }

        public override string Name
        {
            get
            {
                return base.Name + " с копьем";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return base.Attack + 5;
            }
        }

        public override Int32 Defence
        {
            get
            {
                return base.Defence - 1;
            }
        }

        public override Unit CloneModifiedUnit(Int32 healthPoints)
        {
            return new KnightUnitSpearDecorator((KnightUnit)_knight.CloneModifiedUnit(healthPoints));
        }
    }

}
