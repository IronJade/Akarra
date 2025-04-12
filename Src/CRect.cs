using System;

namespace Akarra.Basic
{
    /// <summary>
    /// Primitive rectangle class.
    /// </summary>
    public class Rect
    {
        // Public properties
        public int Left { get; set; }
        public int Top { get; set; }
        public int Right { get; set; }
        public int Bottom { get; set; }
        
        // Constructors
        public Rect()
        {
            Left = 0;
            Top = 0;
            Right = 0;
            Bottom = 0;
        }
        
        public Rect(int left, int top, int right, int bottom)
        {
            Left = left;
            Top = top;
            Right = right;
            Bottom = bottom;
        }
        
        public Rect(Point upperLeft, Point lowerRight)
        {
            Left = upperLeft.X;
            Top = upperLeft.Y;
            Right = lowerRight.X;
            Bottom = lowerRight.Y;
        }
        
        public Rect(Rect rect)
        {
            Left = rect.Left;
            Top = rect.Top;
            Right = rect.Right;
            Bottom = rect.Bottom;
        }
        
        // Operators
        public static Rect operator +(Rect rect, Point point)
        {
            return new Rect(
                rect.Left + point.X,
                rect.Top + point.Y,
                rect.Right + point.X,
                rect.Bottom + point.Y
            );
        }
        
        public static Rect operator -(Rect rect, Point point)
        {
            return new Rect(
                rect.Left - point.X,
                rect.Top - point.Y,
                rect.Right - point.X,
                rect.Bottom - point.Y
            );
        }
        
        // Properties
        public int Width => Right - Left;
        public int Height => Bottom - Top;
        public int Size => Width * Height;
        
        // Methods
        public Point GetTopLeft()
        {
            return new Point(Left, Top);
        }
        
        public Point GetBottomRight()
        {
            return new Point(Right, Bottom);
        }
        
        public Point GetCenterPoint()
        {
            int width = Width;
            int height = Height;
            return new Point(Left + (width >> 1), Top + (height >> 1));
        }
        
        public void ClipRect(Rect rect)
        {
            if (Left < rect.Left) Left = rect.Left;
            if (Top < rect.Top) Top = rect.Top;
            if (Right > rect.Right) Right = rect.Right;
            if (Bottom > rect.Bottom) Bottom = rect.Bottom;
        }
        
        public bool IsWithinRect(Rect rect)
        {
            return (Left >= rect.Left && Top >= rect.Top && Right <= rect.Right && Bottom <= rect.Bottom);
        }
        
        public bool DoesIntersect(Rect rect)
        {
            return !(Left >= rect.Right || Right <= rect.Left || Top >= rect.Bottom || Bottom <= rect.Top);
        }
        
        public bool PointInRect(Point point)
        {
            return (point.X >= Left && point.X < Right && point.Y >= Top && point.Y < Bottom);
        }
        
        public bool PointInRect(int x, int y)
        {
            return (x >= Left && x < Right && y >= Top && y < Bottom);
        }
        
        public void SetRect(int x1, int y1, int x2, int y2)
        {
            Left = x1;
            Top = y1;
            Right = x2;
            Bottom = y2;
        }
        
        public void SetRect(Point topLeft, Point bottomRight)
        {
            Left = topLeft.X;
            Top = topLeft.Y;
            Right = bottomRight.X;
            Bottom = bottomRight.Y;
        }
        
        // Override object methods
        public override bool Equals(object obj)
        {
            if (!(obj is Rect))
                return false;
                
            Rect other = (Rect)obj;
            return Left == other.Left && Top == other.Top && 
                   Right == other.Right && Bottom == other.Bottom;
        }
        
        public override int GetHashCode()
        {
            return Left.GetHashCode() ^ Top.GetHashCode() ^ 
                   Right.GetHashCode() ^ Bottom.GetHashCode();
        }
        
        public override string ToString()
        {
            return $"({Left}, {Top}, {Right}, {Bottom})";
        }
    }
}