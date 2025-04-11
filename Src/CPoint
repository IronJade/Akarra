using System;

namespace Akarra.Basic
{
    /// <summary>
    /// Primitive two-dimensional point class.
    /// </summary>
    public class Point
    {
        // Public variables
        public int X { get; set; }
        public int Y { get; set; }
        
        // Constructors
        public Point()
        {
            X = 0;
            Y = 0;
        }
        
        public Point(int x, int y)
        {
            X = x;
            Y = y;
        }
        
        public Point(Point point)
        {
            X = point.X;
            Y = point.Y;
        }
        
        // Operators
        public static Point operator +(Point a, Point b)
        {
            return new Point(a.X + b.X, a.Y + b.Y);
        }
        
        public static Point operator -(Point a, Point b)
        {
            return new Point(a.X - b.X, a.Y - b.Y);
        }
        
        public static Point operator -(Point a)
        {
            return new Point(-a.X, -a.Y);
        }
        
        public static Point operator *(Point a, int scalar)
        {
            return new Point(a.X * scalar, a.Y * scalar);
        }
        
        public static bool operator ==(Point a, Point b)
        {
            if (ReferenceEquals(a, b))
                return true;
                
            if (ReferenceEquals(a, null) || ReferenceEquals(b, null))
                return false;
                
            return a.X == b.X && a.Y == b.Y;
        }
        
        public static bool operator !=(Point a, Point b)
        {
            return !(a == b);
        }
        
        // Methods
        public void Move(int x, int y)
        {
            X += x;
            Y += y;
        }
        
        public void Capsulize(int x1, int y1, int x2, int y2)
        {
            if (X < x1)
                X = x1;
            else if (X > x2)
                X = x2;
                
            if (Y < y1)
                Y = y1;
            else if (Y > y2)
                Y = y2;
        }
        
        public void Capsulize(Rect rect)
        {
            if (X < rect.Left)
                X = rect.Left;
            else if (X >= rect.Right)
                X = rect.Right - 1;
                
            if (Y < rect.Top)
                Y = rect.Top;
            else if (Y >= rect.Bottom)
                Y = rect.Bottom - 1;
        }
        
        public bool IsWithin(int x1, int y1, int x2, int y2)
        {
            return (X >= x1 && X < x2 && Y >= y1 && Y < y2);
        }
        
        public bool IsWithin(Rect rect)
        {
            return (X >= rect.Left && X < rect.Right && Y >= rect.Top && Y < rect.Bottom);
        }
        
        // Override object methods
        public override bool Equals(object obj)
        {
            if (!(obj is Point))
                return false;
                
            Point other = (Point)obj;
            return X == other.X && Y == other.Y;
        }
        
        public override int GetHashCode()
        {
            return X.GetHashCode() ^ Y.GetHashCode();
        }
        
        public override string ToString()
        {
            return $"({X}, {Y})";
        }
    }
}