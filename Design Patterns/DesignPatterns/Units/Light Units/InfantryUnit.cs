using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Пехотицец.
    /// Если пехотинц стоит в стеке рядом с рыцарем, он становится
    /// его оруженосцем и экипирует рыцаря доспехами или оружием.
    /// </summary>
    class InfantryUnit : LightUnit, ICanBeHealed, IClonableUnit, ISpecialAction
    {
        public override String Name
        {
            get
            {
                return "Infantry";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return 60;
            }
        }
        public override Int32 Defence
        {
            get
            {
                return 20;
            }
        }
        public override Int32 Cost
        {
            get
            {
                return 100;
            }
        }

        public override int Health
        {
            get
            {
                return base.Health;
            }
            protected set
            {
                _health = value > 0 ? (value < MaxHealth ? value : MaxHealth) : 0;
            }
        }

        public InfantryUnit(Int32 health = 100)
        {
            Health = MaxHealth = health;
        }

        #region ICanBeHealed

        public Int32 MaxHealth
        {
            get;
            protected set;
        }

        public UnitKeeper Heal(Int32 healthPoints)
        {
            Unit clone = this.CloneModifiedUnit(healthPoints);
            return new UnitKeeper(this, clone, null);
        }

        #endregion

        #region IClonableUnit

        public UnitKeeper MagicallyClone()
        {
            InfantryUnit clone = (InfantryUnit)this.MemberwiseClone();
            return new UnitKeeper(this, null, clone);
        }

        #endregion

        #region ISpecialAction

        public Int32 Range         { get { return 1; } }
        public Int32 Probability   { get { return 90; } }
        public Int32 Power         { get { return 0; } }

        public ActionTarget Target { get { return ActionTarget.Ally; } }

        public Boolean IsCompatibleType(Unit target)
        {
            return target is KnightUnit;
        }

        /// <summary>
        /// Вооружение рыцаря при наличии доступной амуниции.
        /// (Рыцарь не может сидеть на двух конях.)
        /// </summary>
        /// <param name="target">Рыцарь.</param>
        /// <returns>Объект, инскапсулирующий состояния рыцаря до и после обмундирования.</returns>
        public UnitKeeper SpecialAction(Unit target)
        {
            KnightUnit knight = target as KnightUnit;

            if (knight == null)
                throw new ArgumentException("Target is supposed to be a knight.");

            UnitKeeper targetKeeper = knight.Arm();

            targetKeeper.Log.ObjectName = target.Name;
            targetKeeper.Log.Action = "armed";
            targetKeeper.Log.SubjectName = this.Name.ToLower();

            return targetKeeper;
        }

        #endregion

        public override string ToString()
        {
            return String.Format("I({0})", Health);
        }
    }
}
